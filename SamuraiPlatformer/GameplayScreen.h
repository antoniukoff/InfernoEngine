#pragma once
#include <InfernoEngine/IGameScreen.h>
#include <Box2D/Box2D.h>
#include "Box.h"
#include "Light.h"
#include <vector>
#include <InfernoEngine/SpriteBatch.h>
#include <InfernoEngine/GLSLProgram.h>
#include <InfernoEngine/Camera2D.h>
#include <InfernoEngine/GLTexture.h>
#include <InfernoEngine/Window.h>
#include <InfernoEngine/DebugRenderer.h>
#include <InfernoEngine/GUI.h>
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
	std::vector<Light> m_lights;
	Player m_player;
	
};

