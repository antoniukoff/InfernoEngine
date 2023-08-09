#include "MainGame.h"
#include <string>
#include "Errors.h"


MainGame::MainGame(): _window(nullptr), _screenW(1024), _screenH(768), _gameState(GameState::PLAY), _time(0.0f), max_FPS(60.0f)
{
}

MainGame::~MainGame()
{
}

void MainGame::run()
{
	initSystems();
	_sprites.push_back(new Sprite());
	_sprites.back()->init(0.0f, -1.0f, 1.0f, 1.0f, "Textures/PNG/CharacterRight_Standing.png");

	_sprites.push_back(new Sprite());
	_sprites.back()->init(-1.0f, -1.0f, 1.0f, 1.0f, "Textures/PNG/CharacterRight_Standing.png");

	_sprites.push_back(new Sprite());
	_sprites.back()->init(0.0f, 0.0f, 1.0f, 1.0f, "Textures/PNG/CharacterRight_Standing.png");

	Update();
}

void MainGame::initSystems()
{
	//Initialize SDL
	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

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

	//check for open gl version
	printf("***** OpenGL version: %s *****\n", glGetString(GL_VERSION));

	//set up vsync
	SDL_GL_SetSwapInterval(0);

	glClearColor(0.0f, 0.0f, 1.0f, 1.0f);

	initShaders();
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
	while (_gameState != GameState::EXIT) {
		//used for frame time measuring
		float startTicks = SDL_GetTicks();

		HandleEvents();
		_time += 0.1f;
		drawGame();
		calculateFPS();


		//print every 10 frames
		static int frameCounter = 0;
		frameCounter++;
		if(frameCounter == 10){ 
			std::cout << _fps << std::endl;
			frameCounter = 0;
		}
		
		// limit fps to the max fps
		float frameTicks = SDL_GetTicks() - startTicks;

		if (1000.0f / max_FPS > frameTicks) {
			SDL_Delay(1000.0f / max_FPS - frameTicks);
		}
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

	glActiveTexture(GL_TEXTURE0);
	GLint texturelocation = _colorProgram.getUniformLocation("mySampler");
	glUniform1i(texturelocation, 0);

	GLuint timeLocation = _colorProgram.getUniformLocation("time");
	glUniform1f(timeLocation, _time);
	
	for (int i = 0; i < _sprites.size(); i++) {
		_sprites[i]->draw();
	}

	glBindTexture(GL_TEXTURE_2D, 0);

	_colorProgram.unuse();

	SDL_GL_SwapWindow(_window);


}

void MainGame::calculateFPS()
{
	static const int  NUM_SAMPLES = 10;
	static float frameTimes[NUM_SAMPLES];
	static int currentFrame = 0;

	static float prevTicks = SDL_GetTicks();
	float currentTicks;

	currentTicks = SDL_GetTicks();

	_frameTime = currentTicks - prevTicks;
	frameTimes[currentFrame % NUM_SAMPLES] = _frameTime;

	prevTicks = currentTicks;

	int count;

	currentFrame++;
	if (currentFrame < NUM_SAMPLES) {
		count = currentFrame;
	}
	else {
		count = NUM_SAMPLES;
	}

	float frameTimeAverage = 0;
	for (int i = 0; i < count; i++) {
		frameTimeAverage += frameTimes[i];
	}
	frameTimeAverage /= count;

	if (frameTimeAverage > 0) {
		_fps = 1000.0f / frameTimeAverage;
	}
	else {
		_fps = 70.0f;
	}

	
}
