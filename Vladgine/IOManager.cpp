#include "IOManager.h"

#include <fstream>

namespace Vladgine {

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

}