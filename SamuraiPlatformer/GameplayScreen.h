#pragma once
#include <Vladgine/IGameScreen.h>

class GameplayScreen: public Vladgine::IGameScreen
{
public:
	GameplayScreen();
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
	void checkInput();

};

