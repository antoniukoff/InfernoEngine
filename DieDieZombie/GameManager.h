#pragma once
#include <InfernoEngine/Window.h>
#include <InfernoEngine/GLSLProgram.h>
#include <InfernoEngine/InputManager.h>
#include <InfernoEngine/Camera2D.h>
#include <InfernoEngine/SpriteFont.h>
#include <InfernoEngine/SpriteBatch.h>
#include <InfernoEngine/AudioEngine.h>
#include <InfernoEngine/ParticleEngine2D.h>
#include <InfernoEngine/ParticleBatch2D.h>
#include <InfernoEngine/Timing.h>
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
	void isPlayerWithinCameraLevelBounds();
	

	Inferno::Window m_window;
	Inferno::GLSLProgram m_textureProgram;
	Inferno::InputManager m_inputManager;
	Inferno::Camera2D m_camera;
	Inferno::Camera2D m_hudCamera;
	Inferno::FPSLimiter fpsLimiter;
	std::vector<Level*> m_levels;
	std::vector<Human*> m_humans;
	std::vector<Zombie*> m_zombies;
	std::vector<Bullet> m_bullets;

	Inferno::SpriteBatch m_spriteBatch;
	Inferno::SpriteBatch m_hudBatch;
	Inferno::ParticleEngine2D m_particleEngine;
	Inferno::ParticleBatch2D* m_bloodParticleBatch;

	int m_screenWidth, m_screenHeight;
	float m_fps;
	float m_maxFPS;

	int m_currentLevel;

	Player* m_player;

	int m_numHumansKilled;
	int m_numZombiesKilled;

	Inferno::SpriteFont* m_spriteFont;
	Inferno::AudioEngine m_audioEngine;

	GameState m_gameState;
};

