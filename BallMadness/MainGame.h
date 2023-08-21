#pragma once

#include <Vladgine/Camera2D.h>
#include <Vladgine/SpriteBatch.h>
#include <Vladgine/InputManager.h>
#include <Vladgine/Window.h>
#include <Vladgine/GLSLProgram.h>
#include <Vladgine/Timing.h>
#include <Vladgine/SpriteFont.h>
#include <memory>
#include "Grid.h"

#include "BallController.h"
#include "BallRenderer.h"


// TODO:
// Visualize momentum with color
// Visualize velocity with color
// Visualize position with color

enum class GameState { RUNNING, EXIT };

const int CELL_SIZE = 12;

class MainGame {
public:
    ~MainGame();
    void run();


private:
    void init();
    void initRenderers();
    void initBalls();
    void update(float deltaTime);
    void draw();
    void drawHud();
    void processInput();

    int m_screenWidth = 0;
    int m_screenHeight = 0;

    std::vector<Ball> m_balls; ///< All the balls 
    std::unique_ptr<Grid> m_grid; ///< Grid for spatial partitioning for collision

    int m_currentRenderer = 0;
    BallRenderer m_ballRenderers;

    BallController m_ballController; ///< Controls balls

    Vladgine::Window m_window; ///< The main window
    Vladgine::SpriteBatch m_spriteBatch; ///< Renders all the balls
    std::unique_ptr<Vladgine::SpriteFont> m_spriteFont; ///< For font rendering
    Vladgine::Camera2D m_camera; ///< Renders the scene
    Vladgine::InputManager m_inputManager; ///< Handles input
    Vladgine::GLSLProgram m_textureProgram; ///< Shader for textures]

    Vladgine::FPSLimiter m_fpsLimiter; ///< Limits and calculates fps
    float m_fps = 0.0f;

    GameState m_gameState = GameState::RUNNING; ///< The state of the game
};

