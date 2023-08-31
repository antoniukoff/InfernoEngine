#include "Window.h"
#include "VladgineErrors.h"

namespace Vladgine {

	Window::Window()
	{
	}

	Window::~Window()
	{
	}

	int Window::create(std::string windowName, int screenW, int screenH, unsigned int currentFlags)
	{
		// setting the current flag to open gl
		Uint32 flags = SDL_WINDOW_OPENGL;
		_screenW = screenW;
		_screenH = screenH;

		// if we want to or on some more flags we are going to check if 
		// the flag we pass is going to match the enum flags bitwise
		// if yes we are going to or on that flag to open gl flag

		if (currentFlags & INVISIBLE) {
			flags |= SDL_WINDOW_HIDDEN;
		}
		if (currentFlags & FULLSCREEN) {
			flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
		}
		if (currentFlags & BORDERLESS) {
			flags |= SDL_WINDOW_BORDERLESS;
		}

		_sdlWindow = SDL_CreateWindow(windowName.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenW, screenH, flags);
		if (_sdlWindow == nullptr) {
			fatalError("SDL wingow could ot be created!");
		}

		SDL_GLContext glContext = SDL_GL_CreateContext(_sdlWindow);
		if (glContext == nullptr) {
			fatalError("GLcontext could ot be created!");
		}
		
		// init open gl
		GLenum error = glewInit();
		if (error != GLEW_OK) {
			fatalError("could not init glew");
		}

		//check for open gl version
		printf("***** OpenGL version: %s *****\n", glGetString(GL_VERSION));

		//set up vsync
		SDL_GL_SetSwapInterval(0);

		// set background color
		glClearColor(0.0f, 0.0f, 1.0f, 1.0f);

		//enable alpha blending
		glEnable(GL_BLEND);

		return 0;
	}

	void Window::swapBuffer()
	{
		SDL_GL_SwapWindow(_sdlWindow);
	}

}