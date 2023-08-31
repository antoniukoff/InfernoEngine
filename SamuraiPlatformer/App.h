#pragma once

#include <Vladgine/IMainGame.h>
#include "GameplayScreen.h"
#include "MainMenuScreen.h"
#include "EditorScreen.h"

class GameplayScreen;

class App : public Vladgine::IMainGame
{
public:
	App();
	~App();
	virtual void onInit() override;
	virtual void addScreens() override;
	virtual void onExit() override;
private:
	std::unique_ptr<GameplayScreen> m_gameplayScreen = nullptr;
	std::unique_ptr<MainMenuScreen> m_mainMenuScreen = nullptr;
	std::unique_ptr<EditorScreen> m_editorScreen = nullptr;
};

