#include "GameManager.h"
#include <Vladgine/Vladgine.h>
#include <Vladgine/Timing.h>
#include <Vladgine/Errors.h>
#include "Gun.h"
#include <iostream>
#include "Zombie.h"
#include <random>
#include <ctime>
#include <algorithm>


const float HUMAN_SPEED = 1.0f;
const float ZOMBIE_SPEED = 1.3f;
const float PLAYER_SPEED = 10.0f;

GameManager::GameManager(): _screenWidth(1024), _screenHeight(768), _gameState(GameState::PLAY), _maxFPS(2500.0f),
_player(nullptr), _numZombiesKilled(0), _numHumansKilled(0)
{
}

GameManager::~GameManager()
{
	for (int i = 0; i < _levels.size(); i++) {
		delete _levels[i];
	}
}

void GameManager::run()
{
	initSystems();
	gameLoop();
}

void GameManager::initSystems()
{
	Vladgine::init();

	_window.create("DieDieZombie", _screenWidth, _screenHeight, 0);
	glClearColor(0.6f, 0.6f, 0.6f, 1.0f);

	initShaders();
	initLevel();

	_agentSpriteBatch.init();

	_camera.init(_screenWidth, _screenHeight);	
}

void GameManager::initShaders()
{
	_textureProgram.compileShaders("Shaders/defaultVert.glsl", "Shaders/defaultFrag.glsl");
	_textureProgram.addAttribure("vertexPos");
	_textureProgram.addAttribure("vertexColor");
	_textureProgram.addAttribure("vertUV");
	_textureProgram.linkShaders();
}

void GameManager::initLevel()
{
	_levels.push_back(new Level("Levels/Level2.txt"));
	_currentLevel = 0;

	_player = new Player();
	_player->init(PLAYER_SPEED, _levels[_currentLevel]->getStartPlayerPos(), &_inputManager, &_camera, &_bullets);

	_humans.push_back(_player);
	
	static std::mt19937 randomEngine;
	randomEngine.seed(time(nullptr));
	std::uniform_int_distribution<int> randX(1, _levels[_currentLevel]->getWidth() - 1);
	std::uniform_int_distribution<int> randY(1, _levels[_currentLevel]->getHeight() - 1);

	for (int i = 0; i < _levels[_currentLevel]->getNumHumans(); i++) {
		_humans.push_back(new Human());
		glm::vec2 pos(randX(randomEngine) * TILE_WIDTH, randY(randomEngine) * TILE_WIDTH);
		_humans.back()->init(HUMAN_SPEED, pos);
	}

	const std::vector<glm::vec2>& zombiePositions = _levels[_currentLevel]->getZombieStartPos();
	for (int i = 0; i < zombiePositions.size(); i++) {
		_zombies.push_back(new Zombie());
		_zombies.back()->init(ZOMBIE_SPEED, zombiePositions[i]);
	}
	const float BULLET_SPEED = 20.0f;

	_player->addGun(new Gun("Magnum", 10, 1, 5.0f, 30, BULLET_SPEED));
	_player->addGun(new Gun("Shotgun", 30, 12, 20.0f, 4, BULLET_SPEED));
	_player->addGun(new Gun("MP5", 2, 1, 10.0f, 20 , BULLET_SPEED));

}

void GameManager::gameLoop()
{
	const float DESIRED_FPS = 60.0f;

	const int MAX_PHYSICS_STEPS = 6;

	const float MAX_DELTATIME = 1.0f;

	Vladgine::FPSLimiter fpsLimiter;
	fpsLimiter.setMaxFPS(_maxFPS);

	const float CAMERA_SCALE = 2.0f / 4.0f;
	_camera.setScale(CAMERA_SCALE);

	const float MS_PER_SECOND = 1000.0f;

	const float DESIRED_FRAMETIME = MS_PER_SECOND / DESIRED_FPS;

	float  prevTicks = SDL_GetTicks();
		
	while (_gameState == GameState::PLAY) {
		fpsLimiter.begin();

		float newTicks = SDL_GetTicks();

		float frameTime = newTicks - prevTicks;

		prevTicks = newTicks;

		float totalDeltaTime = frameTime / DESIRED_FRAMETIME;

		checkVictory();

		_inputManager.update();

		processInput();

		int i = 0;

		while (totalDeltaTime > 0.0f && i < MAX_PHYSICS_STEPS) {
			float deltaTime = std::min(totalDeltaTime, MAX_DELTATIME);
			updateAgents(deltaTime);
			updateBullets(deltaTime);


			totalDeltaTime -= deltaTime;
			i++;
		}
		
		

		_camera.setPos(_player->getPosition());
		_camera.update();

		drawGame();

		_fps = fpsLimiter.calculate();

		static int framesCount = 0;
		if (framesCount == 1) {
			std::cout << _fps << std::endl;
			framesCount = 0;
		}
		else
		{
			framesCount++;
		}

	}
}

void GameManager::updateAgents(float deltaTime)
{
	for (int i = 0; i < _humans.size(); i++) {
		_humans[i]->update(_levels[_currentLevel]->getLevelData(), _humans, _zombies, deltaTime);
	}

	for (int i = 0; i < _zombies.size(); i++) {
		_zombies[i]->update(_levels[_currentLevel]->getLevelData(), _humans, _zombies, deltaTime);
	}
	//update Zombie collision
	for (int i = 0; i < _zombies.size(); i++) {
		//collide with other zombies
		for (int j = i + 1; j < _zombies.size(); j++) {
			_zombies[i]->collideWithAgent(_zombies[j]);
		}
		//collide with humans
		for (int j = 1; j < _humans.size(); j++) {
			if (_zombies[i]->collideWithAgent(_humans[j])) {
				_zombies.push_back(new Zombie());
				_zombies.back()->init(ZOMBIE_SPEED, _humans[j]->getPosition());
				delete _humans[j];
				_humans[j] = _humans.back();
				_humans.pop_back();
			}
		}
		//collide with player

		if (_zombies[i]->collideWithAgent(_player)) {
			Vladgine::fatalError("You Lose!");
		}

	}
	// update human collision
	for (int i = 0; i < _humans.size(); i++) {
		for (int j = i+1; j < _humans.size(); j++) {
			_humans[i]->collideWithAgent(_humans[j]);
		}
	}
}

void GameManager::updateBullets(float deltaTime)
{
	for (int i = 0; i < _bullets.size();) {
		if (_bullets[i].update(_levels[_currentLevel]->getLevelData(), deltaTime)) {
			_bullets[i] = _bullets.back();
			_bullets.pop_back();
		}
		else {
			i++;
		}
	}

	bool isBulletRemoved;

	for (int i = 0; i < _bullets.size(); i++) {
		isBulletRemoved = false;
		for (int j = 0; j < _zombies.size(); ) {
			if (_bullets[i].collideWithAgent(_zombies[j])) {

				if (_zombies[j]->applyDamage(_bullets[i].getDamage())) {
					delete _zombies[j];
					_zombies[j] = _zombies.back();
					_zombies.pop_back();
					_numZombiesKilled++;
				}
				else {
					j++;
				}
				_bullets[i] = _bullets.back();
				_bullets.pop_back();
				isBulletRemoved = true;
				i--;// make sure we don't skip the bullets
				//since the bullet died no need to loop through any more zombies
				break;
			}
			else {
				j++;
			}
		}
		if (isBulletRemoved == false) {
			for (int j = 1; j < _humans.size(); ) {
				if (_bullets[i].collideWithAgent(_humans[j])) {

					if (_humans[j]->applyDamage(_bullets[i].getDamage())) {
						delete _humans[j];
						_humans[j] = _humans.back();
						_humans.pop_back();
					}
					else {
						j++;
					}
					_bullets[i] = _bullets.back();
					_bullets.pop_back();
					_numHumansKilled++;
					i--;// make sure we don't skip the bullets
					//since the bullet died no need to loop through any more zombies
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
	if (_zombies.empty()) {

		std::printf("***You win***\n You killed %d humans and %d zombies. There are %d/%d humans remaining",
			_numHumansKilled, _numZombiesKilled, _humans.size() - 1, _levels[_currentLevel]->getNumHumans());

		Vladgine::fatalError("You win!");
	}
}

void GameManager::processInput()
{
	const float CAMERA_SPEED = 10.0f;
	const float SCALE_SPEED = 0.1f;

	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			break;
		case SDL_KEYDOWN:
			_inputManager.pressKey(event.key.keysym.sym);
			break;
			// updates private variables of the _camera
		case SDL_KEYUP:
			_inputManager.releaseKey(event.key.keysym.sym);
			break;
		case SDL_MOUSEBUTTONDOWN:
			_inputManager.pressKey(event.button.button);
			break;
		case SDL_MOUSEBUTTONUP:
			_inputManager.releaseKey(event.button.button);
			break;
		case SDL_MOUSEMOTION:
			_inputManager.setMouseCoords(event.motion.x, event.motion.y);
		}

	}

}

void GameManager::drawGame()
{
	glClearDepth(1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	_textureProgram.use();

	glActiveTexture(GL_TEXTURE0);
	GLint textureUniform = _textureProgram.getUniformLocation("mySampler");
	glUniform1i(textureUniform, 0);

	glm::mat4 projectionMatrix = _camera.getCameraMatrix();
	GLint pUniform = _textureProgram.getUniformLocation("P");
	glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

	_levels[_currentLevel]->draw();

	_agentSpriteBatch.begin();

	// draw humans
	for (int i = 0; i < _humans.size(); i++) {
		_humans[i]->draw(_agentSpriteBatch);
	}

	for (int i = 0; i < _zombies.size(); i++) {
		_zombies[i]->draw(_agentSpriteBatch);
	}	

	for (int i = 0; i < _bullets.size(); i++) {
		_bullets[i].draw(_agentSpriteBatch);
		
	}

	_agentSpriteBatch.end();

	_agentSpriteBatch.renderBatch();

	

	_textureProgram.unuse();

	_window.swapBuffer();
}
