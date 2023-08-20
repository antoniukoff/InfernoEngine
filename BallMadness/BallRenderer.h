#pragma once

#include <Vladgine/SpriteBatch.h>

#include "Ball.h"

// Ball renderer interface
class BallRenderer {
public:
     void renderBalls(Vladgine::SpriteBatch& spriteBatch, Ball& ball);
};
//protected:
//    std::unique_ptr<Vladgine::GLSLProgram> m_program = nullptr;
//};
//
//// Visualizes kinetic energy
//class MomentumBallRenderer : public BallRenderer {
//public:
//    virtual void renderBalls(Vladgine::SpriteBatch& spriteBatch, const std::vector<Ball>& balls,
//                             const glm::mat4& projectionMatrix) override;
//};
//
//// Visualizes positive X component of velocity, as well as position
//class VelocityBallRenderer : public BallRenderer {
//public:
//    VelocityBallRenderer(int screenWidth, int screenHeight);
//
//    virtual void renderBalls(Vladgine::SpriteBatch& spriteBatch, const std::vector<Ball>& balls,
//                             const glm::mat4& projectionMatrix) override;
//private:
//    int m_screenWidth;
//    int m_screenHeight;
//};
//
//// Trippy renderer!
//class TrippyBallRenderer : public BallRenderer {
//public:
//    TrippyBallRenderer(int screenWidth, int screenHeight);
//
//    virtual void renderBalls(Vladgine::SpriteBatch& spriteBatch, const std::vector<Ball>& balls,
//                             const glm::mat4& projectionMatrix) override;
//private:
//    int m_screenWidth;
//    int m_screenHeight;
//    float m_time = 0.0f;
//};