#include "MainGame.h"
#include <string>
#include "Errors.h"


MainGame::MainGame(): _window(nullptr), _screenW(1024), _screenH(768), _gameState(GameState::PLAY)
{
}

MainGame::~MainGame()
{
}

void MainGame::run()
{
	initSystems();

	_sprite.init(-1.0f, -1.0f, 1.0f, 1.0f);

	Update();
}

void MainGame::initSystems()
{
	//Initialize SDL
	SDL_Init(SDL_INIT_EVERYTHING);

	_window = SDL_CreateWindow("Game Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, _screenW, _screenH, SDL_WINDOW_OPENGL);
	if (_window == nullptr) {
		fatalError("SDL wingow could ot be created!");
	}

	SDL_GLContext glContext = SDL_GL_CreateContext(_window);
	if (glContext == nullptr) {
		fatalError("GLcontext could ot be created!");
	}

	GLenum error = glewInit();
	if (error != GLEW_OK) {
		fatalError("could not init glew");
	}

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	glClearColor(0.0f, 0.0f, 1.0f, 1.0f);

	initShaders();
}

void MainGame::initShaders()
{
	_colorProgram.compileShaders("Shaders/defaultVert.glsl", "Shaders/defaultFrag.glsl");
	_colorProgram.addAttribure("vertexPos");
	_colorProgram.linkShaders();
}

void MainGame::Update()
{
	while (_gameState != GameState::EXIT) {
		HandleEvents();
		drawGame();
	}
}

void MainGame::HandleEvents()
{
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			_gameState = GameState::EXIT;
			break;
		case SDL_MOUSEMOTION:
		
			break;
		}
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

	_sprite.draw();

	_colorProgram.unuse();

	SDL_GL_SwapWindow(_window);

}
