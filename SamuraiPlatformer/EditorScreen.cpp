#include "EditorScreen.h"
#include <Vladgine/Vertex.h>
#include <Vladgine/ResourceManager.h>

EditorScreen::EditorScreen(Vladgine::Window* window) : 
	m_window(window),
	m_spriteFont("Fonts/chintzy.ttf", 32)
{
	m_screenIndex = SCREEN_INDEX_EDITOR;
}

EditorScreen::~EditorScreen()
{

}

int EditorScreen::getNextScreenIndex() const
{
	return SCREEN_INDEX_NO_SCREEN;
}

int EditorScreen::getPreviousScreenIndex() const
{
	return SCREEN_INDEX_MAINMENU;
}

void EditorScreen::build()
{

}

void EditorScreen::destroy()
{

}

void EditorScreen::onEntry()
{

	m_camera.init(m_window->getScreenWidth(), m_window->getScreenHeight());
	m_camera.setScale(1.0f);
	initUI();


	m_spriteBatch.init();
	m_textureProgram.compileShaders("Shaders/defaultVert.glsl", "Shaders/defaultFrag.glsl");
	m_textureProgram.addAttribure("vertexPos");
	m_textureProgram.addAttribure("vertexColor");
	m_textureProgram.addAttribure("vertUV");
	m_textureProgram.linkShaders();

	m_blankTexture = Vladgine::ResourceManager::getTexture("Textures/blank.png");
}

void EditorScreen::onExit()
{
	m_gui.destroy();
}

void EditorScreen::update()
{
	m_camera.update();
	checkInput();
}

void EditorScreen::draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(1.0f, 0.0f, 1.0f, 1.0f);

	m_textureProgram.use();

	// Upload texture uniform
	GLint textureUniform = m_textureProgram.getUniformLocation("mySampler");
	glUniform1i(textureUniform, 0);
	glActiveTexture(GL_TEXTURE0);

	// Camera matrix
	glm::mat4 projectionMatrix = m_camera.getCameraMatrix();
	GLint pUniform = m_textureProgram.getUniformLocation("P");
	glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);
	m_spriteBatch.begin();
	{
		const float QUAD_SIZE = 75.0f;
	
		glm::vec4 destRect;
		destRect.x = m_bSlider->getXPosition().d_scale * m_window->getScreenWidth() + 20.0f - m_window->getScreenWidth() / 2.0f + QUAD_SIZE / 2.0f;
		destRect.y = m_window->getScreenHeight() / 2.0f - m_bSlider->getYPosition().d_scale * m_window->getScreenHeight() -
			m_bSlider->getHeight().d_scale * m_window->getScreenHeight() * 0.5 - QUAD_SIZE /2.0f;
		destRect.z = QUAD_SIZE;
		destRect.w = QUAD_SIZE;
		m_spriteBatch.draw(destRect, glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), m_blankTexture.id, 0.0f, Vladgine::ColorRGB8(m_colorPickerRed, m_colorPickerGreen, m_colorPickerBlue, 255));
		
	}

	{
	
		glm::vec2 position;
		position.x = m_rigidRadioButton->getXPosition().d_scale * m_window->getScreenWidth() - m_window->getScreenWidth() / 2.0f + 
			m_rigidRadioButton->getWidth().d_offset / 2.0f;
		position.y = m_window->getScreenHeight() / 2.0f - m_rigidRadioButton->getYPosition().d_scale * m_window->getScreenHeight();
		m_spriteFont.draw(m_spriteBatch, "Rigid", position, glm::vec2(0.5f), 0.0f, Vladgine::ColorRGB8(255,255,255,255),Vladgine::Justification::MIDDLE);

		position.x = m_dynamicRadioButton->getXPosition().d_scale * m_window->getScreenWidth() - m_window->getScreenWidth() / 2.0f +
			m_dynamicRadioButton->getHeight().d_offset / 2.0f;
		position.y = m_window->getScreenHeight() / 2.0f - m_dynamicRadioButton->getYPosition().d_scale * m_window->getScreenHeight();
		m_spriteFont.draw(m_spriteBatch, "Dynamic", position, glm::vec2(0.5f), 0.0f, Vladgine::ColorRGB8(255, 255, 255, 255), Vladgine::Justification::MIDDLE);
	}
	m_spriteBatch.end();
	m_spriteBatch.renderBatch();

	m_textureProgram.unuse();

	m_gui.draw();

	glEnable(GL_BLEND);
}

void EditorScreen::initUI()
{
	m_gui.init("GUI");
	m_gui.loadScheme("TaharezLook.scheme");
	m_gui.setFont("DejaVuSans-10");

	
	{ // add color picker
		const float X_DIM = 0.015f, Y_DIM = 0.1f;
		const float Y_POS = 0.05f;

		m_rSlider = static_cast<CEGUI::Slider*>(m_gui.createWidget("TaharezLook/Slider", glm::vec4(0.05f, 0.05f, X_DIM, Y_DIM), glm::vec4(0.0f), "rSlider"));
		m_rSlider->setMaxValue(255.0f);
		m_rSlider->setCurrentValue(m_colorPickerRed);
		m_rSlider->subscribeEvent(CEGUI::Slider::EventValueChanged, CEGUI::Event::Subscriber(&EditorScreen::onColorPickerRedChanged, this));
		m_rSlider->setClickStep(1.0f);

		m_gSlider = static_cast<CEGUI::Slider*>(m_gui.createWidget("TaharezLook/Slider", glm::vec4(0.07f, 0.05f, X_DIM, Y_DIM), glm::vec4(0.0f), "gSlider"));
		m_gSlider->setMaxValue(255.0f);
		m_gSlider->setCurrentValue(m_colorPickerRed);
		m_gSlider->subscribeEvent(CEGUI::Slider::EventValueChanged, CEGUI::Event::Subscriber(&EditorScreen::onColorPickerGreenChanged, this));
		m_gSlider->setClickStep(1.0f);

		m_bSlider = static_cast<CEGUI::Slider*>(m_gui.createWidget("TaharezLook/Slider", glm::vec4(0.09f, 0.05f, X_DIM, Y_DIM), glm::vec4(0.0f), "bSlider"));
		m_bSlider->setMaxValue(255.0f);
		m_bSlider->setCurrentValue(m_colorPickerRed);
		m_bSlider->subscribeEvent(CEGUI::Slider::EventValueChanged, CEGUI::Event::Subscriber(&EditorScreen::onColorPickerBlueChanged, this));
		m_bSlider->setClickStep(1.0f);
	}

	{
		const float Y_POS = 0.25f;
		const float DIMS_PIXELS = 20.0f;

		
		m_rigidRadioButton = static_cast<CEGUI::RadioButton*>(m_gui.createWidget("TaharezLook/RadioButton", glm::vec4(0.05f, Y_POS, 0.0f, 0.0f), glm::vec4(0.0f, 0.0f, DIMS_PIXELS, DIMS_PIXELS), "rigidButton"));
		m_rigidRadioButton->setSelected(true);
		m_rigidRadioButton->subscribeEvent(CEGUI::RadioButton::EventMouseClick, CEGUI::Event::Subscriber(&EditorScreen::onRigidMouseClick, this));


		m_dynamicRadioButton = static_cast<CEGUI::RadioButton*>(m_gui.createWidget("TaharezLook/RadioButton", glm::vec4(0.05f + 0.05 + 0.00025f, Y_POS, 0.0f, 0.0f), glm::vec4(0.0f, 0.0f, DIMS_PIXELS, DIMS_PIXELS), "dynamicButton"));
		m_dynamicRadioButton->setSelected(false);
		m_dynamicRadioButton->subscribeEvent(CEGUI::RadioButton::EventMouseClick, CEGUI::Event::Subscriber(&EditorScreen::onDynamicMouseClick, this));


	}




	m_gui.setMouseCursor("TaharezLook/MouseArrow");
	m_gui.showMouseCursor();
	SDL_ShowCursor(0);
}

void EditorScreen::checkInput()
{
	SDL_Event evnt;
	while (SDL_PollEvent(&evnt))
	{
		m_gui.onSDLEvent(evnt);
		switch (evnt.type)
		{
		case SDL_QUIT:
			onExitClicked(CEGUI::EventArgs());
		default:
			break;
		}
	}
}


bool EditorScreen::onExitClicked(const CEGUI::EventArgs& e)
{
	m_currentState = Vladgine::Screen_State::CHANGE_PREVIOUS;

	return true;
}

bool EditorScreen::onColorPickerRedChanged(const CEGUI::EventArgs& e)
{
	m_colorPickerRed = m_rSlider->getCurrentValue();
	return true;
}

bool EditorScreen::onColorPickerGreenChanged(const CEGUI::EventArgs& e)
{
	m_colorPickerGreen = m_gSlider->getCurrentValue();
	return true;
}

bool EditorScreen::onColorPickerBlueChanged(const CEGUI::EventArgs& e)
{
	m_colorPickerBlue = m_bSlider->getCurrentValue();
	return true;
}

bool EditorScreen::onRigidMouseClick(const CEGUI::EventArgs& e)
{
	m_physicsMode = PhysicsMode::RIGID;
	return true;
}

bool EditorScreen::onDynamicMouseClick(const CEGUI::EventArgs& e)
{
	m_physicsMode = PhysicsMode::DYNAMIC;
	return true;
}
