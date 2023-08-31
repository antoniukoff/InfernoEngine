#pragma once
#pragma once
#include <Vladgine/IGameScreen.h>
#include "Box.h"
#include <vector>
#include <Vladgine/SpriteBatch.h>
#include <Vladgine/SpriteFont.h>
#include <Vladgine/GLSLProgram.h>
#include <Vladgine/Camera2D.h>
#include <Vladgine/Window.h>
#include <Vladgine/GUI.h>
#include "ScreenIndices.h"

enum class PhysicsMode {
	RIGID,
	DYNAMIC
};

class EditorScreen : public Vladgine::IGameScreen
{
public:
	EditorScreen(Vladgine::Window* window);
	~EditorScreen();


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
	bool onColorPickerRedChanged(const CEGUI::EventArgs& e);
	bool onColorPickerGreenChanged(const CEGUI::EventArgs& e);
	bool onColorPickerBlueChanged(const CEGUI::EventArgs& e);
	bool onRigidMouseClick(const CEGUI::EventArgs& e);
	bool onDynamicMouseClick(const CEGUI::EventArgs& e);

	float m_colorPickerRed = 255.0f;
	float m_colorPickerGreen = 255.0f;
	float m_colorPickerBlue = 255.0f;

	CEGUI::Slider* m_rSlider = nullptr;
	CEGUI::Slider* m_gSlider = nullptr;
	CEGUI::Slider* m_bSlider = nullptr;
	CEGUI::RadioButton* m_rigidRadioButton = nullptr;
	CEGUI::RadioButton* m_dynamicRadioButton = nullptr;

	Vladgine::GLTexture m_blankTexture;

	PhysicsMode m_physicsMode = PhysicsMode::RIGID;

	Vladgine::SpriteBatch m_spriteBatch;
	Vladgine::SpriteFont m_spriteFont;
	Vladgine::GLSLProgram m_textureProgram;
	Vladgine::Camera2D m_camera;
	Vladgine::Window* m_window;
	Vladgine::GUI m_gui;
};