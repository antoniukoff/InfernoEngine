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

class MainMenuScreen : public Inferno::IGameScreen
{
public:
	MainMenuScreen(Inferno::Window* window);
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
	Inferno::Camera2D m_camera;
	Inferno::Window* m_window;
	Inferno::GUI m_gui;
};