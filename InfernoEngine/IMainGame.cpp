#include "IMainGame.h"
#include "Timing.h"
#include "Inferno.h"
#include "IGameScreen.h"
#include "ScreenList.h"

Inferno::IMainGame::IMainGame()
{
	m_screenList = std::make_unique<ScreenList>(this);
}

Inferno::IMainGame::~IMainGame()
{
	//empty
}

void Inferno::IMainGame::run()
{

	if (!init()) return;

	FPSLimiter limiter;
	limiter.setMaxFPS(60.0f);

	m_isRunning = true;
	while (m_isRunning) {
		limiter.begin();

		m_inputManager.update();

		update();

		if (m_isRunning) {
			draw();
		}

		m_fps = limiter.calculate();
		m_window.swapBuffer();
	}
}

void Inferno::IMainGame::exitGame()
{
	m_currentScreen->onExit();
	if (m_screenList) {
		m_screenList->destroy();
		m_screenList.reset();
	}
	m_isRunning = false;
}

void Inferno::IMainGame::update()
{
	if (m_currentScreen) {
		switch (m_currentScreen->getState())
		{
		case Screen_State::RUNNING:
			m_currentScreen->update();
			break;
		case Screen_State::CHANGE_NEXT:
			m_currentScreen->onExit();
			m_currentScreen = m_screenList->moveNext();
			if (m_currentScreen) {
				m_currentScreen->setRunning();
				m_currentScreen->onEntry();
			}
			break;
		case Screen_State::CHANGE_PREVIOUS:
			m_currentScreen->onExit();
			m_currentScreen = m_screenList->movePrevious();
			if (m_currentScreen) {
				m_currentScreen->setRunning();
				m_currentScreen->onEntry();
			}
			break;
		case Screen_State::EXIT_APPLICATION:
			exitGame();
			break;
		default:
			break;
		}
	}
	else {
		exitGame();
	}
}

void Inferno::IMainGame::draw()
{
	glViewport(0, 0, m_window.getScreenWidth(), m_window.getScreenHeight());

	if (m_currentScreen && m_currentScreen->getState() == Screen_State::RUNNING) {
		m_currentScreen->draw();
	}
}

void Inferno::IMainGame::onSDLEvent(SDL_Event& event)
{

	switch (event.type) {
	case SDL_QUIT:
		break;
	case SDL_MOUSEMOTION:
		m_inputManager.setMouseCoords((float)event.motion.x, (float)event.motion.y);
		break;
	case SDL_KEYDOWN:
		m_inputManager.pressKey(event.key.keysym.sym);
		break;
	case SDL_KEYUP:
		m_inputManager.releaseKey(event.key.keysym.sym);
		break;
	case SDL_MOUSEBUTTONDOWN:
		m_inputManager.pressKey(event.button.button);
		break;
	case SDL_MOUSEBUTTONUP:
		m_inputManager.releaseKey(event.button.button);
		break;
	}
	
}

bool Inferno::IMainGame::init()
{
	Inferno::init();

	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

	if (!initSystems()) return false;

	onInit();
	addScreens();

	m_currentScreen = m_screenList->getCurrent();
	m_currentScreen->onEntry();
	m_currentScreen->setRunning();

	return true;
}

bool Inferno::IMainGame::initSystems()
{
	m_window.create("Default", 1280, 720, 0);

	return true;
}
