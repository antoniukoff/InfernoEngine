#pragma once
#pragma once
#include <InfernoEngine/IGameScreen.h>
#include "Box.h"
#include <vector>
#include <InfernoEngine/SpriteBatch.h>
#include <InfernoEngine/GLSLProgram.h>
#include <InfernoEngine/Camera2D.h>
#include <InfernoEngine/Window.h>
#include <InfernoEngine/GUI.h>
#include "ScreenIndices.h"

class MainMenuScreen : public Vladgine::IGameScreen
{
public:
	MainMenuScreen(Vladgine::Window* window);
	~MainMenuScreen();


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
	bool onEditorEntry(const CEGUI::EventArgs& e);
	bool onNewGameEntry(const CEGUI::EventArgs& e);

	int m_nextScreenIndex = SCREEN_INDEX_GAMEPLAY;
	Vladgine::Camera2D m_camera;
	Vladgine::Window* m_window;
	Vladgine::GUI m_gui;
};