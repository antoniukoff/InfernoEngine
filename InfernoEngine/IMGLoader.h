#pragma once
#include "GLTexture.h"
#include <string>
#include "picoPNG.h"

namespace Inferno {

	class IMGLoader
	{
	public:
		static GLTexture loadPNG(std::string filePath);
	};

}


