#pragma once
#include <vector>
#include <string>

namespace Inferno {


	struct DirEntry{
		std::string path;
		bool isDirectory;
	};

	class IOManager
	{
	public:

		///reads from a file and fills up a buffer with an information from the file
		/// I use static because i need one instance of it
		static bool reafFileToBuffer(std::string filePath, std::vector<unsigned char>& buffer);
		static bool reafFileToBuffer(std::string filePath, std::string& buffer);
		static bool getDirectoryEntries(const char* path, std::vector<DirEntry>& rvEntries);
		static bool makeDiretory(const char* path);
	};

}