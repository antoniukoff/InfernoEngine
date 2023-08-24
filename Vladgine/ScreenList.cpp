#include "ScreenList.h"
#include "IGameScreen.h"

namespace Vladgine {

    ScreenList::ScreenList(IMainGame* game): m_game(game)
    {
        //empty
    }

    ScreenList::~ScreenList()
    {
        destroy();
    }

    IGameScreen* Vladgine::ScreenList::moveNext()
    {
        IGameScreen* currenScreen = getCurrent();
        if (currenScreen->getNextScreenIndex() != SCREEN_INDEX_NO_SCREEN) {
            m_currentScreenIndex = currenScreen->getNextScreenIndex();
        }
        return getCurrent();
    }

    IGameScreen* Vladgine::ScreenList::movePrevious()
    {
		IGameScreen* currenScreen = getCurrent();
		if (currenScreen->getPreviousScreenIndex() != SCREEN_INDEX_NO_SCREEN) {
			m_currentScreenIndex = currenScreen->getPreviousScreenIndex();
		}
		return getCurrent();
    }

    void ScreenList::setScreen(int nextScreen)
    {
        m_currentScreenIndex = nextScreen;
    }

    void ScreenList::addScreen(IGameScreen* newScreen)
    {
        newScreen->m_screenIndex = m_screens.size();
        m_screens.push_back(newScreen);
        newScreen->build();
        newScreen->setParentGame(m_game);
    }

    void ScreenList::destroy()
    {
        for (size_t i = 0; i < m_screens.size(); i++) {
			m_screens[i]->destroy();
        }
        m_screens.resize(0);
        m_currentScreenIndex = SCREEN_INDEX_NO_SCREEN;
    }

    IGameScreen* ScreenList::getCurrent()
    {
        if (m_currentScreenIndex == SCREEN_INDEX_NO_SCREEN) return nullptr;
        return m_screens[m_currentScreenIndex];
    }
}