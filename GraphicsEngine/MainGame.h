#pragma once

#include <iostream>
#include <SDL.h>
#include <glew.h>
#include "Sprite.h"
#include "GLSLProgram.h"

using namespace std;

enum class GameState {PLAY, EXIT};

class MainGame
{
public:
	MainGame();
	~MainGame();

	void run();

private:
	void initSystems();
	void initShaders();
	void Update();
	void HandleEvents();
	void drawGame();

	SDL_Window* _window;
	int _screenW;
	int _screenH;
	GameState _gameState;

	Sprite _sprite; 
	GLSLProgram _colorProgram;
};

