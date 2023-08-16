
#include "MainGame.h"
#include "MainGame.h"
#include <string>
#include <Vladgine/Errors.h>
#include <Vladgine/Vladgine.h>


MainGame::MainGame() : _screenW(1024), _screenH(768), _gameState(GameState::PLAY), _time(0.0f), max_FPS(60.0f)
{
	camera.init(_screenW, _screenH);
}

MainGame::~MainGame()
{
}

void MainGame::run()
{
	initSystems();

	Update();
}

void MainGame::initSystems()
{
	//Initialize SDL
	Vladgine::init();

	_window.create("Vladgine", _screenW, _screenH, 0);

	initShaders();

	spriteBatch.init();

	_fpsLimiter.init(max_FPS);
}

void MainGame::initShaders()
{
	_colorProgram.compileShaders("Shaders/defaultVert.glsl", "Shaders/defaultFrag.glsl");
	_colorProgram.addAttribure("vertexPos");
	_colorProgram.addAttribure("vertexColor");
	_colorProgram.addAttribure("vertUV");
	_colorProgram.linkShaders();
}


void MainGame::Update()
{
	//game loop
	while (_gameState != GameState::EXIT) {

		//used for frame time measuring
		_fpsLimiter.begin();
		HandleEvents();
		_time += 0.01f;

		//Updates the camera position and in case of the event(Moves everything in the opposite dir)
		camera.update();

		// update bullets
		for (int i = 0; i < _bullets.size();) {
			if (_bullets[i].update() == true) {
				_bullets[i] = _bullets.back();
				_bullets.pop_back();

			}
			else {
				i++;
			}
		}

		drawGame();
		_fps = _fpsLimiter.calculate();

		//print every 10 frames
		static int frameCounter = 0;
		frameCounter++;
		if (frameCounter == 1000) {
			std::cout << _fps << std::endl;
			frameCounter = 0;
		}
	}
}

void MainGame::HandleEvents()
{

	const float CAMERA_SPEED = 10.0f;
	const float SCALE_SPEED = 0.1f;

	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			_gameState = GameState::EXIT;
			break;
		case SDL_KEYDOWN:
			_inputManager.pressKey(event.key.keysym.sym);
			break;
			// updates private variables of the camera
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
	if (_inputManager.isKeyDown(SDLK_w)) {
		camera.setPos(camera.getPos() + glm::vec2(0.0, 1.0 * CAMERA_SPEED));
	}
	if (_inputManager.isKeyDown(SDLK_s)) {
		camera.setPos(camera.getPos() + glm::vec2(0.0, -1.0 * CAMERA_SPEED));
	}
	if (_inputManager.isKeyDown(SDLK_d)) {
		camera.setPos(camera.getPos() + glm::vec2(CAMERA_SPEED, 0.0));
	}
	if (_inputManager.isKeyDown(SDLK_a)) {
		camera.setPos(camera.getPos() + glm::vec2(-CAMERA_SPEED, 0.0));
	}
	if (_inputManager.isKeyDown(SDLK_q)) {
		camera.setScale(camera.getScale() + SCALE_SPEED);
	}
	if (_inputManager.isKeyDown(SDLK_e)) {
		camera.setScale(camera.getScale() - SCALE_SPEED);
	}
	if (_inputManager.isKeyDown(SDL_BUTTON_LEFT)) {
		glm::vec2 mouseCoords = _inputManager.getMouseCoords();
		mouseCoords = camera.converScreenToWorld(mouseCoords);
		glm::vec2 playerPosition(0.0f);
		glm::vec2 direction = mouseCoords - playerPosition;
		direction = glm::normalize(direction);

		_bullets.emplace_back(playerPosition, direction, 1.0f, 1000);
	}
}

//draws the game using OpenGl
void MainGame::drawGame()
{

	//set the base depth to 1.0f
	glClearDepth(1.0);
	//clear the buffer and the depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	_colorProgram.use();

	glActiveTexture(GL_TEXTURE0);
	GLint texturelocation = _colorProgram.getUniformLocation("mySampler");
	glUniform1i(texturelocation, 0);


	//set the camera matrix
	// set the uniform variable to the updated camera matrix in the draw call.
	GLint pLocation = _colorProgram.getUniformLocation("P");
	glm::mat4 cameraMatrix = camera.getCameraMatrix();
	glUniformMatrix4fv(pLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));

	spriteBatch.begin();

	glm::vec4 position(0, 0, 50, 50);
	glm::vec4 uv(0.0f, 0.0f, 1.0f, 1.0f);
	static Vladgine::GLTexture texture = Vladgine::ResourceManager::getTexture("Textures/PNG/CharacterRight_Standing.png");
	Vladgine::ColorRGB8 color;
	color.r = 255;
	color.g = 255;
	color.b = 255;
	color.a = 255;

	spriteBatch.draw(position, uv, texture.id, 0.0f, color);

	for (int i = 0; i < _bullets.size(); i++) {
		_bullets[i].draw(spriteBatch);
	}

	spriteBatch.end();
	spriteBatch.renderBatch();

	glBindTexture(GL_TEXTURE_2D, 0);

	_colorProgram.unuse();

	_window.swapBuffer();


}


