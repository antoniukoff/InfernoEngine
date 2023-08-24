#include "App.h"
#include <Vladgine/ScreenList.h>


App::App()
{

}

App::~App()
{

}

void App::onInit()
{

}

void App::addScreens()
{
	m_gameplayScreen = std::make_unique<GameplayScreen>();

	m_screenList->addScreen(m_gameplayScreen.get());
	m_screenList->setScreen(m_gameplayScreen->getIndex());
}

void App::onExit()
{

}