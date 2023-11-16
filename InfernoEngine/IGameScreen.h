#pragma once

#define SCREEN_INDEX_NO_SCREEN -1

namespace Inferno {

	class IMainGame;

	enum class Screen_State {
		NONE,
		RUNNING,
		CHANGE_NEXT,
		CHANGE_PREVIOUS,
		EXIT_APPLICATION
	};

	class IGameScreen
	{
	public:
		friend class ScreenList;
		IGameScreen() {
			//empty
		}
		virtual ~IGameScreen() {
			//empty
		}

		//return the index ofthe next or previous screen when changing screens
		virtual int getNextScreenIndex() const = 0;
		virtual int getPreviousScreenIndex() const = 0;


		//called at the beginning and end of app
		virtual void build() = 0;
		virtual void destroy() = 0;

		//called when a screen enters and exits focus
		virtual void onEntry() = 0;
		virtual void onExit() = 0;

		virtual void update() = 0;
		virtual void draw() = 0;

		int getIndex() const {
			return m_screenIndex;
		}

		void setRunning() { m_currentState = Screen_State::RUNNING; }

		Screen_State getState() const { return m_currentState; }

		void setParentGame(IMainGame* game) { m_game = game; }

	protected:
		Screen_State m_currentState = Screen_State::NONE;
		IMainGame* m_game = nullptr;

		int m_screenIndex = -1;

	};
}
