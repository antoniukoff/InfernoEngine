#pragma once

#include <iostream>
#include <SDL.h>
#include <glew.h>
#include "Sprite.h"
#include "GLSLProgram.h"
#include "GLTexture.h"
#include <vector>

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

	void calculateFPS();

	SDL_Window* _window;
	int _screenW;
	int _screenH;
	GameState _gameState;

	std::vector<Sprite*> _sprites; 

	GLSLProgram _colorProgram;

	float _time;

	float _fps;
	float _frameTime;
	float max_FPS;
};

