#include "Errors.h"
#include <iostream>
#include<SDL.h>

void fatalError(std::string errorString) {
	std::cout << errorString << std::endl;
	std::cout << "Enter any key to quit..." << std::endl;
	int tmp;
	std::cin >> tmp;
	SDL_Quit();
}