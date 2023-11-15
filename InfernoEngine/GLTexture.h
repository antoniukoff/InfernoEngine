#pragma once
#include <GLEW/glew.h>
#include <string>
namespace Vladgine {

	struct GLTexture {
		std::string filePath = "";
		GLuint id;
		int width, height;
	};

}
