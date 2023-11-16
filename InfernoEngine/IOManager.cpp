#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING

#include "IOManager.h"

#include <fstream>
#include <experimental/filesystem>

namespace fs = std::experimental::filesystem;

namespace Inferno {

    bool IOManager::reafFileToBuffer(std::string filePath, std::vector<unsigned char>& buffer)
    {
        std::ifstream file(filePath, std::ios::binary);
        if (file.fail()) {
            perror(filePath.c_str());
            return false;
        }

        //seek to the end
        file.seekg(0, std::ios::end);

        //get file size
        int fileSize = file.tellg();

        //seek to the beginning after we stored the file size
        file.seekg(0, std::ios::beg);


        //remove the file header bytes from the actual file size
        fileSize -= file.tellg();

        buffer.resize(fileSize);
        file.read((char*)&(buffer[0]), fileSize);

        file.close();
        return true;
    }

    bool IOManager::reafFileToBuffer(std::string filePath, std::string& buffer)
    {
		std::ifstream file(filePath, std::ios::binary);
		if (file.fail()) {
			perror(filePath.c_str());
			return false;
		}

		//seek to the end
		file.seekg(0, std::ios::end);

		//get file size
		int fileSize = file.tellg();

		//seek to the beginning after we stored the file size
		file.seekg(0, std::ios::beg);


		//remove the file header bytes from the actual file size
		fileSize -= file.tellg();

		buffer.resize(fileSize);
		file.read((char*)&(buffer[0]), fileSize);

		file.close();
		return true;
    }

    bool IOManager::getDirectoryEntries(const char* path, std::vector<DirEntry>& rvEntries)
    {
        auto dpath = fs::path(path);
        
        if (!fs::is_directory(dpath)) return false;
        for (auto it = fs::directory_iterator(dpath); it != fs::directory_iterator(); ++it) {
            rvEntries.emplace_back();
            rvEntries.back().path = it->path().string();
            if (is_directory(it->path())) {
                rvEntries.back().isDirectory = true;
            }
            else {
                rvEntries.back().isDirectory = false;
            }
        }
        return true;
    }

	bool IOManager::makeDiretory(const char* path)
	{
        return fs::create_directory(fs::path(path));
	}

}