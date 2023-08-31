#pragma once
#include <Vladgine/IGameScreen.h>
#include <Box2D/Box2D.h>
#include "Box.h"
#include <vector>
#include <Vladgine/SpriteBatch.h>
#include <Vladgine/GLSLProgram.h>
#include <Vladgine/Camera2D.h>
#include <Vladgine/GLTexture.h>
#include <Vladgine/Window.h>
#include <Vladgine/DebugRenderer.h>
#include <Vladgine/GUI.h>
#include "Player.h"

class GameplayScreen : public Vladgine::IGameScreen
{
public:
	GameplayScreen(Vladgine::Window* window);
	~GameplayScreen();


	// Inherited via IGameScreen
	virtual int getNextScreenIndex() const override;

	virtual int getPreviousScreenIndex() const override;

	virtual void build() override;

	virtual void destroy() override;

	virtual void onEntry() override;

	virtual void onExit() override;

	virtual void update() override;

	virtual void draw() override;

private:
	void initUI();
	void checkInput();

	bool onExitClicked(const CEGUI::EventArgs& e);

	Vladgine::SpriteBatch m_spriteBatch;
	Vladgine::GLSLProgram m_textureProgram;
	Vladgine::GLSLProgram m_lightProgram;
	Vladgine::Camera2D m_camera;
	Vladgine::GLTexture m_texture;
	Vladgine::Window* m_window;
	Vladgine::DebugRenderer m_debugRenderer;
	Vladgine::GUI m_gui;

	bool m_renderDebug = false;

	std::unique_ptr<b2World> m_world;
	std::vector<Box> m_boxes;
	Player m_player;
};

