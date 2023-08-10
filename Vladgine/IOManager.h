#pragma once
#include <vector>
#include <string>

namespace Vladgine {

	class IOManager
	{
	public:

		///reads from a file and fills up a buffer with an information from the file
		/// I use static because i need one instance of it
		static bool reafFileToBuffer(std::string filePath, std::vector<unsigned char>& buffer);
	};

}