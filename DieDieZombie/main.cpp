#include <SDL/SDL.h>
#include <GLEW/glew.h>
#include <iostream>
#include "GameManager.h"

int main(int argc, char** argv)
{
   
    GameManager gameManager;
    gameManager.run();

    return 0;
}
