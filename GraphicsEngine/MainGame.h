#pragma once

#include <iostream>
#include <vector>
#include <Vladgine/Sprite.h>
#include <Vladgine/GLSLProgram.h>
#include <Vladgine/GLTexture.h>
#include <Vladgine/Window.h>
#include <Vladgine/Camera2D.h>
#include <Vladgine/SpriteBatch.h>
#include <Vladgine/ResourceManager.h>

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

	Vladgine::Window _window;
	int _screenW;
	int _screenH;
	GameState _gameState;

	Vladgine::GLSLProgram _colorProgram;
	Vladgine::Camera2D camera;
	Vladgine::SpriteBatch spriteBatch;

	float _time;

	float _fps;
	float _frameTime;
	float max_FPS;
};

