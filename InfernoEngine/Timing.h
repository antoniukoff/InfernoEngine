#pragma once

namespace Inferno {

	class FPSLimiter {
	public:

		FPSLimiter();
		void init(float fps);
		void setMaxFPS(float fps);
		void begin();

		//will return the current fps
		float calculate();
	private:
		void calculateFps();
		float _maxFps;


		float _fps;
		float _frameTime;
		unsigned int _startTicks;
	};

}