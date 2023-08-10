#include "MainGame.h"
#include <string>
#include <Vladgine/Errors.h>
#include <Vladgine/Vladgine.h>


MainGame::MainGame(): _screenW(1024), _screenH(768), _gameState(GameState::PLAY), _time(0.0f), max_FPS(60.0f)
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

	_window.create("Vladgine", _screenW, _screenH, Vladgine::BORDERLESS);

	initShaders();

	spriteBatch.init();
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
		float startTicks = SDL_GetTicks();

		HandleEvents();
		_time += 0.01f;

		//Updates the camera position and in case of the event(Moves everything in the opposite dir)
		camera.update();

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
		
		//if it takes less time to process the game loop we are going to delay the 
		//frametime by the difference between the desired framerite and the current framerate
		//When we call calculatefps() we update the currentTicks by calling sdl_getticks function
		//the function does not stop calculating time even after we called sdl_delay and is
		//resulting in a bigger time difference between the frames, which in turn going to calculate
		//lower frame rate per 10 samples

		float frameTicks = SDL_GetTicks() - startTicks;

		if (1000.0f / max_FPS > frameTicks) {
			SDL_Delay(1000.0f / max_FPS - frameTicks);
		}
	}
}

void MainGame::HandleEvents()
{

	const float CAMERA_SPEED = 20.0f;
	const float SCALE_SPEED = 0.1f;

	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			_gameState = GameState::EXIT;
			break;
		case SDL_MOUSEMOTION:
		
			break;
		case SDL_KEYDOWN:
			// updates private variables of the camera
			switch (event.key.keysym.sym) {
			case SDLK_w:
				camera.setPos(camera.getPos() + glm::vec2(0.0, 1.0 * CAMERA_SPEED));
				break;
			case SDLK_s:
				camera.setPos(camera.getPos() + glm::vec2(0.0, -1.0 * CAMERA_SPEED));
				break;
			case SDLK_d:
				camera.setPos(camera.getPos() + glm::vec2(CAMERA_SPEED, 0.0));
				break;
			case SDLK_a:
				camera.setPos(camera.getPos() + glm::vec2(-CAMERA_SPEED, 0.0));
				break;
			case SDLK_q:
				camera.setScale(camera.getScale() + SCALE_SPEED);
				break;
			case SDLK_e:
				camera.setScale(camera.getScale() - SCALE_SPEED);
				break;
			}
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

	
	//set the camera matrix
	GLuint timeLocation = _colorProgram.getUniformLocation("time");
	glUniform1f(timeLocation, _time);
	
	// set the uniform variable to the updated camera matrix in the draw call.
	GLint pLocation = _colorProgram.getUniformLocation("P");
	glm::mat4 cameraMatrix = camera.getCameraMatrix();
	glUniformMatrix4fv(pLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));
			
	spriteBatch.begin();

	glm::vec4 position(0, 0, 50, 50);
	glm::vec4 uv(0.0f, 0.0f, 1.0f, 1.0f);
	Vladgine::GLTexture texture = Vladgine::ResourceManager::getTexture("Textures/PNG/CharacterRight_Standing.png");
	Vladgine::Color color;
	color.r = 255;
	color.g = 255;
	color.b = 255;
	color.a = 255;


	spriteBatch.draw(position, uv, texture.id, 0.0f, color);

	
	spriteBatch.end();
	spriteBatch.renderBatch();

	glBindTexture(GL_TEXTURE_2D, 0);

	_colorProgram.unuse();

	_window.swapBuffer();


}

void MainGame::calculateFPS()
{

	// initialize variables only once at the start of the program
	static const int  NUM_SAMPLES = 10;
	static float frameTimes[NUM_SAMPLES];
	static int currentFrame = 0;

	static float prevTicks = SDL_GetTicks();

	// have to reset it every frame by using updated ticks
	float currentTicks;
	currentTicks = SDL_GetTicks();

	//calculate frame time(how long it took to process all the game loop code from the previous code to the current code)
	_frameTime = currentTicks - prevTicks;

	// creating a circular buffer - updates frame time for each sample by calculating the modulo
	frameTimes[currentFrame % NUM_SAMPLES] = _frameTime;

	// set the prev ticks to current ticks to keep track of the prevTicks in the next frame(the next time the function is called)
	prevTicks = currentTicks;
	int count;
	currentFrame++;

	//if there are less then 10 samples in the buffer avarage the existing once, otherwise avarages 10 samles(if 20 frames has passed still 10 samples going to be avaraged)
	if (currentFrame < NUM_SAMPLES) {
		count = currentFrame;
	}
	else {
		count = NUM_SAMPLES;
	}
	
	//sum up all the existing frametimes of all samlples and divide by the count
	float frameTimeAverage = 0;
	for (int i = 0; i < count; i++) {
		frameTimeAverage += frameTimes[i];
	}
	frameTimeAverage /= count;

	// calculate the fps
	if (frameTimeAverage > 0) {
		_fps = 1000.0f / frameTimeAverage;
	}
	else {
		_fps = 70.0f;
	}

	
}
