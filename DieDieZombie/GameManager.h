#pragma once
#include <Vladgine/Window.h>
#include <Vladgine/GLSLProgram.h>
#include <Vladgine/InputManager.h>
#include <Vladgine/Camera2D.h>
#include <Vladgine/SpriteBatch.h>
#include "Bullet.h"
#include "Level.h"
#include "Player.h"

enum class GameState{PLAY, EXIT};

class Zombie;

class GameManager
{
public:
	GameManager();
	~GameManager();

	void run();

private:
	void initSystems();
	void initShaders();
	void initLevel();
	void gameLoop();
	void updateAgents(float deltaTime);
	void updateBullets(float deltaTime);
	void checkVictory();
	void processInput();
	void drawGame();
	

	Vladgine::Window _window;
	Vladgine::GLSLProgram _textureProgram;
	Vladgine::InputManager _inputManager;
	Vladgine::Camera2D _camera;
	std::vector<Level*> _levels;
	std::vector<Human*> _humans;
	std::vector<Zombie*> _zombies;
	std::vector<Bullet> _bullets;

	Vladgine::SpriteBatch _agentSpriteBatch;

	int _screenWidth, _screenHeight;
	float _fps;
	float _maxFPS;

	int _currentLevel;

	Player* _player;

	int _numHumansKilled;
	int _numZombiesKilled;

	GameState _gameState;
};

