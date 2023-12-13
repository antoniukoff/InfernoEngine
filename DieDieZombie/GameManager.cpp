#include <iostream>
#include <random>
#include <ctime>
#include <algorithm>
#include <mutex>
#include <thread>
#include <GLEW/glm/gtx/rotate_vector.hpp>

#include "GameManager.h"
#include "Gun.h"
#include "Zombie.h"

#include <InfernoEngine/Inferno.h>
#include <InfernoEngine/InfernoErrors.h>
#include <InfernoEngine/ResourceManager.h>
#include <InfernoEngine/Core.h>

constexpr float DEG_TO_RAD = 3.14159265359f / 180.0f;
constexpr float RAD_TO_DEG = 180.0f / 3.14159265359f;

std::mutex agentMutex;
std::mutex bulletMutex;

// PoolAllocator Initialization
InfernoEngine::PoolAllocator Human::allocator{ 10 };

// GameManager Constructor
GameManager::GameManager() : m_screenWidth(1920), m_screenHeight(1080), m_gameState(GameState::PLAY), m_maxFPS(2500.0f),
m_player(nullptr), m_numZombiesKilled(0), m_numHumansKilled(0) {
}

// GameManager Destructor
GameManager::~GameManager() {
	for (int i = 0; i < m_humans.size(); i++) {
		delete m_humans[i];
	}
	for (int i = 0; i < m_zombies.size(); i++) {
		delete m_zombies[i];
	}
	for (int i = 0; i < m_levels.size(); i++) {
		delete m_levels[i];
	}
	delete m_spriteFont;
	printMemory();
}

// GameManager Methods
void GameManager::run() {
	initSystems();
	initLevel();
	Inferno::Music music = m_audioEngine.loadMusic("Sounds/Secunda.mp3");
	music.play(-1);
	gameLoop();
}

void GameManager::initSystems() {
	const float CAMERA_SCALE = 2.0f / 4.0f;

	Inferno::init();
	m_audioEngine.init();
	m_window.create("DieDieZombie", m_screenWidth, m_screenHeight, 0);
	
	initShaders();
	m_spriteBatch.init();
	m_hudBatch.init();
	m_spriteFont = new Inferno::SpriteFont("Fonts/cCaesarSalad.ttf", 32);

	// Initialize camera
	m_camera.init(m_screenWidth, m_screenHeight);
	m_camera.setScale(CAMERA_SCALE);

	// Only need to initialize the HUD camera once
	m_hudCamera.init(m_screenWidth, m_screenHeight);
	m_hudCamera.setPos(glm::vec2(m_screenWidth / 2, m_screenHeight / 2));
	m_hudCamera.update();

	m_bloodParticleBatch = new Inferno::ParticleBatch2D;
	m_bloodParticleBatch->init(1000, 0.05f, Inferno::ResourceManager::getTexture("Textures/circle.png"),
		[](Inferno::Particle2D& particle, float deltaTime) {
			particle.position += particle.velocity * deltaTime;
			particle.color.a = (GLubyte)(particle.life * 255.0f);
		});
	m_particleEngine.addParticleBatch(m_bloodParticleBatch);

	fpsLimiter.setMaxFPS(m_maxFPS);
}

void GameManager::initShaders()
{
	m_textureProgram.compileShaders("Shaders/defaultVert.glsl", "Shaders/defaultFrag.glsl");
	m_textureProgram.addAttribure("vertexPos");
	m_textureProgram.addAttribure("vertexColor");
	m_textureProgram.addAttribure("vertUV");
	m_textureProgram.linkShaders();
}

void GameManager::initLevel()
{
	const float HUMAN_SPEED = 1.0f;
	const float ZOMBIE_SPEED = 1.3f;
	const float PLAYER_SPEED = 10.0f;
	const float BULLET_SPEED = 20.0f;

	// Load the level
	m_levels.push_back(new Level("Levels/Level2.txt"));
	m_currentLevel = 0;

	// Initialize the player
	m_player = new Player();
	m_player->init(PLAYER_SPEED, m_levels[m_currentLevel]->getStartPlayerPos(), &m_inputManager, &m_camera, &m_bullets);
	m_humans.push_back(m_player);

	// Seed the random engine for NPC positioning
	static std::mt19937 randomEngine(time(nullptr));
	std::uniform_int_distribution<int> randX(1, m_levels[m_currentLevel]->getWidth() - 1);
	std::uniform_int_distribution<int> randY(1, m_levels[m_currentLevel]->getHeight() - 1);

	// Initialize non-player humans
	for (int i = 0; i < m_levels[m_currentLevel]->getNumHumans(); i++) {
		glm::vec2 pos(randX(randomEngine) * TILE_WIDTH, randY(randomEngine) * TILE_WIDTH);
		Human* human = new Human();
		human->init(HUMAN_SPEED, pos);
		m_humans.push_back(human);
	}

	// Initialize zombies
	const std::vector<glm::vec2>& zombiePositions = m_levels[m_currentLevel]->getZombieStartPos();
	for (const auto& pos : zombiePositions) {
		Zombie* zombie = new Zombie();
		zombie->init(ZOMBIE_SPEED, pos);
		m_zombies.push_back(zombie);
	}

	// Set up player guns
	m_player->addGun(new Gun("Magnum", 10, 1, 5.0f, 30, BULLET_SPEED, m_audioEngine.loadSoundEffect("Sounds/Shots/powerup_02.wav")));
	m_player->addGun(new Gun("Shotgun", 60, 12, 20.0f, 150, BULLET_SPEED, m_audioEngine.loadSoundEffect("Sounds/Shots/gun_shot_01.wav")));
	m_player->addGun(new Gun("MP5", 1, 1, 10.0f, 20, BULLET_SPEED, m_audioEngine.loadSoundEffect("Sounds/Shots/gun_machine_01.wav")));
}


void GameManager::gameLoop()
{
	// Constants
	const int MAX_PHYSICS_STEPS = 6;
	const float DESIRED_FPS = 60.0f;
	const float MAX_DELTATIME = 1.0f;
	const float MS_PER_SECOND = 1000.0f;
	const float DESIRED_FRAMETIME = MS_PER_SECOND / DESIRED_FPS;

	float prevTicks = SDL_GetTicks();

	// Game loop
	while (m_gameState == GameState::PLAY) {
		fpsLimiter.begin();
		float newTicks = SDL_GetTicks();
		float frameTime = newTicks - prevTicks;
		prevTicks = newTicks;
		float totalDeltaTime = frameTime / DESIRED_FRAMETIME;

		checkVictory();
		m_inputManager.update();
		processInput();

		int i = 0;

		// Update the game until the physics is caught up
		while (totalDeltaTime > 0.0f && i < MAX_PHYSICS_STEPS) {
			float deltaTime = std::min(totalDeltaTime, MAX_DELTATIME);

			std::thread updateAgentsThread(&GameManager::updateAgents, this, deltaTime);
			std::thread updateBulletsThread(&GameManager::updateBullets, this, deltaTime);


			updateAgentsThread.join();
			updateBulletsThread.join();

			m_particleEngine.update(deltaTime);

			totalDeltaTime -= deltaTime;
			i++;
		}

		m_camera.setPos(m_player->getPosition());
		isCameraWithinLevelBounds();
        m_camera.update();

        drawGame();
        m_fps = fpsLimiter.calculate();

        static int framesCount = 0;
        if (framesCount++ == 200) {
            
            framesCount = 0;
        }
	}
}

void GameManager::updateAgents(float deltaTime) {
	// Lock to ensure thread safety as agents are updated
	std::lock_guard<std::mutex> lock(agentMutex);

	// Update all humans in the game
	for (int i = 0; i < m_humans.size(); i++) {
		m_humans[i]->update(m_levels[m_currentLevel]->getLevelData(), m_humans, m_zombies, deltaTime);
	}

	// Update all zombies in the game
	for (int i = 0; i < m_zombies.size(); i++) {
		m_zombies[i]->update(m_levels[m_currentLevel]->getLevelData(), m_humans, m_zombies, deltaTime);
	}

	// Check and handle collisions between zombies and other agents
	for (int i = 0; i < m_zombies.size(); i++) {
		// Check for collisions with other zombies
		for (int j = i + 1; j < m_zombies.size(); j++) {
			m_zombies[i]->collideWithAgent(m_zombies[j]);
		}

		// Check for collisions with humans and turn them into zombies if collided
		for (int j = 1; j < m_humans.size(); j++) {
			if (m_zombies[i]->collideWithAgent(m_humans[j])) {
				// Create new zombie at the human's position
				m_zombies.push_back(new Zombie());
				m_zombies.back()->init(1.3f, m_humans[j]->getPosition());
				// Remove the human from the game
				delete m_humans[j];
				m_humans[j] = m_humans.back();
				m_humans.pop_back();
			}
		}

		// Check collision with the player
		if (m_zombies[i]->collideWithAgent(m_player)) {
			Inferno::fatalError("You Lose!");
		}
	}

	// Check for collisions between humans
	for (int i = 0; i < m_humans.size(); i++) {
		for (int j = i + 1; j < m_humans.size(); j++) {
			m_humans[i]->collideWithAgent(m_humans[j]);
		}
	}
}

void GameManager::updateBullets(float deltaTime) {

	std::lock_guard<std::mutex> lock(bulletMutex);

	// Update all bullets and remove them if they are no longer active
	for (int i = 0; i < m_bullets.size();) {
		if (m_bullets[i].update(m_levels[m_currentLevel]->getLevelData(), deltaTime)) {
			m_bullets[i] = m_bullets.back();
			m_bullets.pop_back();
		}
		else {
			i++;
		}
	}

	bool isBulletRemoved;

	// Check collisions for each bullet
	for (int i = 0; i < m_bullets.size(); i++) {
		isBulletRemoved = false;
		for (int j = 0; j < m_zombies.size();) {
			if (m_bullets[i].collideWithAgent(m_zombies[j])) {
				// Add blood effect
				addBlood(m_bullets[i].getPosition(), 50);

				// Apply damage to zombie and remove if necessary
				if (m_zombies[j]->applyDamage(m_bullets[i].getDamage())) {
					delete m_zombies[j];
					m_zombies[j] = m_zombies.back();
					m_zombies.pop_back();
					m_numZombiesKilled++;
				}
				else {
					j++;
				}

				// Remove the bullet as it hit a zombie
				m_bullets[i] = m_bullets.back();
				m_bullets.pop_back();
				isBulletRemoved = true;
				i--; // Decrement to ensure no bullet is skipped
				break;
			}
			else {
				j++;
			}
		}

		// Check collisions with humans if the bullet hasn't been removed
		if (!isBulletRemoved) {
			for (int j = 1; j < m_humans.size();) {
				if (m_bullets[i].collideWithAgent(m_humans[j])) {
					// Add blood effect
					addBlood(m_bullets[i].getPosition(), 50);

					// Apply damage to human and remove if necessary
					if (m_humans[j]->applyDamage(m_bullets[i].getDamage())) {
						delete m_humans[j];
						m_humans[j] = m_humans.back();
						m_humans.pop_back();
						m_numHumansKilled++;
					}
					else {
						j++;
					}

					// Remove the bullet as it hit a human
					m_bullets[i] = m_bullets.back();
					m_bullets.pop_back();
					i--; // Decrement to ensure no bullet is skipped
					break;
				}
				else {
					j++;
				}
			}
		}
	}
}


void GameManager::checkVictory()
{
	//todo support for multiple levels
	if (m_zombies.empty()) {

		std::printf("***You win***\n You killed %d humans and %d zombies. There are %d/%d humans remaining",
			m_numHumansKilled, m_numZombiesKilled, m_humans.size() - 1, m_levels[m_currentLevel]->getNumHumans());

		Inferno::fatalError("You win!");
	}
}

void GameManager::processInput()
{
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			m_gameState = GameState::EXIT;
			break;
		case SDL_KEYDOWN:
			m_inputManager.pressKey(event.key.keysym.sym);
			break;
			// updates private variables of the _camera
		case SDL_KEYUP:
			m_inputManager.releaseKey(event.key.keysym.sym);
			break;
		case SDL_MOUSEBUTTONDOWN:
			m_inputManager.pressKey(event.button.button);
			break;
		case SDL_MOUSEBUTTONUP:
			m_inputManager.releaseKey(event.button.button);
			break;
		case SDL_MOUSEMOTION:
			m_inputManager.setMouseCoords(event.motion.x, event.motion.y);
			break;
		case SDL_MOUSEWHEEL:
			if(event.wheel.y > 0)
				m_camera.setScale(m_camera.getScale() + 0.01f);
			else if(event.wheel.y < 0)
				m_camera.setScale(m_camera.getScale() - 0.01f);
		} 
	}
}

void GameManager::drawGame() {
	glClearDepth(1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_textureProgram.use();

	// Upload texture uniform
	glActiveTexture(GL_TEXTURE0);
	GLint textureUniform = m_textureProgram.getUniformLocation("mySampler");
	glUniform1i(textureUniform, 0);

	// Upload camera matrix
	glm::mat4 projectionMatrix = m_camera.getCameraMatrix();
	GLint pUniform = m_textureProgram.getUniformLocation("P");
	glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

	m_levels[m_currentLevel]->draw();

	m_spriteBatch.begin();
	glm::vec2 agentDims(AGENT_RADIUS * 2.0f);

	// Draw humans
	for (int i = 0; i < m_humans.size(); i++) {
		if (m_camera.isBoxInView(m_humans[i]->getPosition(), agentDims))
			m_humans[i]->draw(m_spriteBatch);
	}

	// Draw zombies
	for (int i = 0; i < m_zombies.size(); i++) {
		if (m_camera.isBoxInView(m_zombies[i]->getPosition(), agentDims))
			m_zombies[i]->draw(m_spriteBatch);
	}

	// Draw bullets
	for (int i = 0; i < m_bullets.size(); i++) {
		m_bullets[i].draw(m_spriteBatch);
	}

	m_spriteBatch.end();
	m_spriteBatch.renderBatch();

	// Render particles
	m_particleEngine.draw(&m_spriteBatch);

	drawHUD();

	m_textureProgram.unuse();
	m_window.swapBuffer();
}

void GameManager::drawHUD() {
	char buffer[256];
	glm::mat4 projectionMatrix = m_hudCamera.getCameraMatrix();
	GLint pUniform = m_textureProgram.getUniformLocation("P");
	glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

	const double MAX_LEAK = 1500000;
	double scaledValue = static_cast<double>(memoryAllocated - memoryDeleted) / MAX_LEAK;

	GLuint barTexture = Inferno::ResourceManager::getTexture("Textures/circle.png").id;
	m_hudBatch.begin();
	glm::vec4 destRect(0, 96, 64, 512 * scaledValue);
	glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
	Inferno::ColorRGB8 color;

	// Set color based on scaled value
	if (scaledValue < 0.58) {
		color = Inferno::ColorRGB8(0, 255, 0, 255);
	}
	else if (scaledValue < 0.61) {
		color = Inferno::ColorRGB8(255, 255, 0, 255);
	}
	else if (scaledValue < 0.62) {
		color = Inferno::ColorRGB8(255, 165, 0, 255);
	}
	else {
		color = Inferno::ColorRGB8(255, 0, 0, 255);
	}

	m_hudBatch.draw(destRect, uvRect, 0, 0, color);
	sprintf_s(buffer, "Num Humans %d", m_humans.size());
	m_spriteFont->draw(m_hudBatch, buffer, glm::vec2(0, 0), glm::vec2(1.0f), 0.0f, Inferno::ColorRGB8(255, 255, 255, 255));

	sprintf_s(buffer, "Num Zombies %d", m_zombies.size());
	m_spriteFont->draw(m_hudBatch, buffer, glm::vec2(0, 36), glm::vec2(1.0f), 0.0f, Inferno::ColorRGB8(255, 255, 255, 255));

	m_hudBatch.end();
	m_hudBatch.renderBatch();
}


void GameManager::addBlood(const glm::vec2& position, int numParticles)
{
	static std::mt19937 randEngine(time(nullptr));
	static std::uniform_real_distribution<float> randAngle(0.0f, 360);

	glm::vec2 vel(2.0f, 0.0f);

	Inferno::ColorRGB8 col(255, 0, 0, 255);

	for (int i = 0; i < numParticles; i++) {
		m_bloodParticleBatch->addParticle(position, glm::rotate(vel, randAngle(randEngine) * DEG_TO_RAD) , col, 15.0f);
	}
}

void GameManager::isCameraWithinLevelBounds() {
	if (m_camera.getPos().x - m_window.getScreenWidth() / 2 / m_camera.getScale() < 0) {
		m_camera.setPos(glm::vec2(m_window.getScreenWidth() / 2 / m_camera.getScale(), m_camera.getPos().y));
	}
	if (m_camera.getPos().x + m_window.getScreenWidth() / 2 / m_camera.getScale() > m_levels[m_currentLevel]->getWidth() * TILE_WIDTH) {
		m_camera.setPos(glm::vec2(m_levels[m_currentLevel]->getWidth() * TILE_WIDTH - m_window.getScreenWidth() / 2 / m_camera.getScale(), m_camera.getPos().y));
	}
	if (m_camera.getPos().y - m_window.getScreenHeight() / 2 / m_camera.getScale() < 0) {
		m_camera.setPos(glm::vec2(m_camera.getPos().x, m_window.getScreenHeight() / 2 / m_camera.getScale()));
	}
	if (m_camera.getPos().y + m_window.getScreenHeight() / 2 / m_camera.getScale() > m_levels[m_currentLevel]->getHeight() * TILE_WIDTH) {
		m_camera.setPos(glm::vec2(m_camera.getPos().x, m_levels[m_currentLevel]->getHeight() * TILE_WIDTH - m_window.getScreenHeight() / 2 / m_camera.getScale()));
	}
	if(m_camera.getScale() < 0.5f)
		m_camera.setScale(0.5f);
}

