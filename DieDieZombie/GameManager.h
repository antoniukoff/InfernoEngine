#pragma once
#include <Vladgine/Window.h>
#include <Vladgine/GLSLProgram.h>
#include <Vladgine/InputManager.h>
#include <Vladgine/Camera2D.h>
#include <Vladgine/SpriteFont.h>
#include <Vladgine/SpriteBatch.h>
#include <Vladgine/AudioEngine.h>
#include <Vladgine/ParticleEngine2D.h>
#include <Vladgine/ParticleBatch2D.h>
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
	void drawHUD();
	void addBlood(const glm::vec2& position, int numParticles);
	

	Vladgine::Window m_window;
	Vladgine::GLSLProgram m_textureProgram;
	Vladgine::InputManager m_inputManager;
	Vladgine::Camera2D m_camera;
	Vladgine::Camera2D m_hudCamera;
	std::vector<Level*> m_levels;
	std::vector<Human*> m_humans;
	std::vector<Zombie*> m_zombies;
	std::vector<Bullet> m_bullets;

	Vladgine::SpriteBatch m_agentSpriteBatch;
	Vladgine::SpriteBatch m_hudSpriteBatch;
	Vladgine::ParticleEngine2D m_particleEngine;
	Vladgine::ParticleBatch2D* m_bloodParticleBatch;

	int m_screenWidth, m_screenHeight;
	float m_fps;
	float m_maxFPS;

	int m_currentLevel;

	Player* m_player;

	int m_numHumansKilled;
	int m_numZombiesKilled;

	Vladgine::SpriteFont* m_spriteFont;
	Vladgine::AudioEngine m_audioEngine;

	GameState m_gameState;
};

