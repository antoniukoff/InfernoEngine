#include "Timing.h"
#include <SDL.h>

namespace Vladgine {

	FPSLimiter::FPSLimiter()
	{
	}

	void FPSLimiter::init(float maxFPS)
	{
		setMaxFPS(maxFPS);
	}

	void FPSLimiter::setMaxFPS(float maxFPS)
	{
		_maxFps = maxFPS;
	}

	void FPSLimiter::begin()
	{
		_startTicks = SDL_GetTicks();
	}

	float FPSLimiter::calculate()
	{
		calculateFps();

		// limit fps to the max fps
		float frameTicks = SDL_GetTicks() - _startTicks;

		//if it takes less time to process the game loop we are going to delay the 
		//frametime by the difference between the desired framerite and the current framerate
		//When we call calculatefps() we update the currentTicks by calling sdl_getticks function
		//the function does not stop calculating time even after we called sdl_delay and is
		//resulting in a bigger time difference between the frames, which in turn going to calculate
		//lower frame rate per 10 samples
		if (1000.0f / _maxFps > frameTicks) {
			SDL_Delay(1000.0f / _maxFps - frameTicks);
		}

		return _fps;
	}

	void FPSLimiter::calculateFps()
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

}