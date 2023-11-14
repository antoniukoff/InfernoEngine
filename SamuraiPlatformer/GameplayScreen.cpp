#include "GameplayScreen.h"
#include <iostream>
#include <Vladgine/IMainGame.h>
#include <Vladgine/ResourceManager.h>
#include "Light.h"
#include "Box.h"
#include <random>
#include "ScreenIndices.h"
#include "LevelMediator.h"



GameplayScreen::GameplayScreen(Vladgine::Window* window) : m_window(window) {

	m_screenIndex = SCREEN_INDEX_GAMEPLAY;

}

GameplayScreen::~GameplayScreen()
{
}

int GameplayScreen::getNextScreenIndex() const
{
	return SCREEN_INDEX_NO_SCREEN;
}

int GameplayScreen::getPreviousScreenIndex() const
{
	return SCREEN_INDEX_MAINMENU;
}

void GameplayScreen::build()
{
}

void GameplayScreen::destroy()
{
}

void GameplayScreen::onEntry()
{
	LevelData levelToLoad = LevelMediator::getInstance()->getLevelData();
	b2Vec2 gravity(0.0f, -25.0f);
	m_world = std::make_unique<b2World>(gravity);
	m_texture = Vladgine::ResourceManager::getTexture("Textures/brick_wall.png");

	const int numBoxes = levelToLoad.boxes.size();;
	for (int i = 0; i < numBoxes; i++) {
		Box newBox;
		newBox.init(m_world.get(), levelToLoad.boxes[i].position,
			levelToLoad.boxes[i].dimensions, m_texture,
			levelToLoad.boxes[i].color,
			levelToLoad.boxes[i].fixedRotation, levelToLoad.boxes[i].isDynamic,
			levelToLoad.boxes[i].angle, levelToLoad.boxes[i].uvRect);
	
		m_boxes.push_back(newBox);
	}

	
	

	//init spritebatch
	m_spriteBatch.init();

	//texture init 
	m_textureProgram.compileShaders("Shaders/defaultVert.glsl", "Shaders/defaultFrag.glsl");
	m_textureProgram.addAttribure("vertexPos");
	m_textureProgram.addAttribure("vertexColor");
	m_textureProgram.addAttribure("vertUV");
	m_textureProgram.linkShaders();

	//lighting init 
	m_lightProgram.compileShaders("Shaders/lightVert.glsl", "Shaders/lightFrag.glsl");
	m_lightProgram.addAttribure("vertexPos");
	m_lightProgram.addAttribure("vertexColor");
	m_lightProgram.addAttribure("vertUV");
	m_lightProgram.linkShaders();

	//load the texture;
	

	//init camera
	m_camera.init(m_window->getScreenWidth(), m_window->getScreenHeight());
	m_camera.setScale(32.0f);

	m_debugRenderer.init();
	//init player
	m_player.init(m_world.get(), levelToLoad.player.position, levelToLoad.player.drawDims, levelToLoad.player.collisionDims, levelToLoad.player.color);
	initUI();
}

void GameplayScreen::onExit()
{
	m_debugRenderer.dispose();
	m_boxes.clear();
	m_world.reset();
}

void GameplayScreen::update()
{

	m_camera.update();
	checkInput();
	m_player.update(m_game->m_inputManager);

	// Update the physics simulation
	m_world->Step(1.0f / 60.0f, 6, 2);
}
	
	

void GameplayScreen::draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	m_textureProgram.use();

	// Upload texture uniform
	GLint textureUniform = m_textureProgram.getUniformLocation("mySampler");
	glUniform1i(textureUniform, 0);
	glActiveTexture(GL_TEXTURE0);

	// Camera matrix
	glm::mat4 projectionMatrix = m_camera.getCameraMatrix();
	GLint pUniform = m_textureProgram.getUniformLocation("P");
	glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

	m_spriteBatch.begin();

	// Draw all the boxes
	for (auto& b : m_boxes) {
		b.draw(m_spriteBatch);
	}
	m_player.draw(m_spriteBatch);


	m_spriteBatch.end();
	m_spriteBatch.renderBatch();
	m_textureProgram.unuse();
	glm::vec4 destRect;
	if (m_renderDebug) {
		for (auto& b : m_boxes) {
			
			destRect.x = b.getBody()->GetPosition().x - b.getDimensions().x / 2;
			destRect.y = b.getBody()->GetPosition().y - b.getDimensions().y / 2;
			destRect.z = b.getDimensions().x;
			destRect.w = b.getDimensions().y;
			m_debugRenderer.drawBox(destRect, Vladgine::ColorRGB8(255, 255, 255, 255), b.getBody()->GetAngle());

			//
		}
		for (auto& l : LevelMediator::getInstance()->getLevelData().light) {
			m_debugRenderer.drawCircle(glm::vec2(l.position.x, l.position.y), Vladgine::ColorRGB8(255, 255, 255, 255), l.size / 2.0f);
		}

		m_player.drawDebug(m_debugRenderer);

		m_debugRenderer.end();
		m_debugRenderer.render(projectionMatrix, 2.0f);
	}
	//render some test lights
	Light playerLight;
	playerLight.color = Vladgine::ColorRGB8(255, 255, 255, 128);
	playerLight.position = m_player.getPosition();
	playerLight.size = 30.0f;

	Light mouseLight;
	mouseLight.color = Vladgine::ColorRGB8(255, 0, 255, 150);
	glm::vec2 mousePos = m_camera.converScreenToWorld(m_game->m_inputManager.getMouseCoords());
	mouseLight.position = mousePos;
	mouseLight.size = 15.0f;

	

	m_lightProgram.use();

	glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	m_spriteBatch.begin();

	pUniform = m_textureProgram.getUniformLocation("P");
	glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

	playerLight.draw(m_spriteBatch);
	mouseLight.draw(m_spriteBatch);
	
	for (auto& l : LevelMediator::getInstance()->getLevelData().light) {
		l.draw(m_spriteBatch);
	}

	m_spriteBatch.end();
	m_spriteBatch.renderBatch();

	m_lightProgram.unuse();
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	m_gui.draw();
	
	glEnable(GL_BLEND);
}

void GameplayScreen::initUI()
{
	//init GUI
	m_gui.init("GUI");
	m_gui.loadScheme("TaharezLook.scheme");
	m_gui.setFont("DejaVuSans-10");
	CEGUI::PushButton* testButton = static_cast<CEGUI::PushButton*>(m_gui.createWidget("TaharezLook/Button", glm::vec4(0.01f, 0.01f, 0.1f, 0.05f), glm::vec4(0.0f), "TestButton"));
	testButton->setText("Exit Game!");

	//set the event to be called when we click 
	testButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GameplayScreen::onExitClicked, this));

	
	m_gui.setMouseCursor("TaharezLook/MouseArrow");
	m_gui.showMouseCursor();
	SDL_ShowCursor(0);
}

void GameplayScreen::checkInput()
{
	SDL_Event evnt;
	while (SDL_PollEvent(&evnt))
	{
		m_game->onSDLEvent(evnt);
		m_gui.onSDLEvent(evnt);
		switch (evnt.type) {
		case SDL_QUIT:
			onExitClicked(CEGUI::EventArgs());
			break;
		}
	}
}

bool GameplayScreen::onExitClicked(const CEGUI::EventArgs& e)
{
	m_currentState = Vladgine::Screen_State::CHANGE_PREVIOUS;

	return true;
}
