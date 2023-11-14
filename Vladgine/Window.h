#pragma once
#include <SDL/SDL.h>
#include <GLEW/glew.h>
#include <string>

namespace Vladgine {

	enum WindowFlags
	{
		INVISIBLE = 0x1, FULLSCREEN = 0x2, BORDERLESS = 0x4
	};

	class Window
	{
	public:
		Window();
		~Window();

		int create(std::string windowName, int screenW, int screenH, unsigned int currentFlags);

		void swapBuffer();

		int getScreenWidth() { return _screenW; }
		int getScreenHeight() { return _screenH; }
	private:
		SDL_Window* _sdlWindow;
		int _screenW, _screenH;

	};

}