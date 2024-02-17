#pragma once
#include <InfernoEngine/SpriteBatch.h>
#include <InfernoEngine/GLSLProgram.h>
#include <vector>
#include "Ball.h"
#include <memory>
#include <map>
#include <queue>


// Ball renderer interface
class BallRenderer {
public:

    virtual void renderBalls(Inferno::SpriteBatch& spriteBatch, 
                             std::vector<Ball>& ball, const glm::mat4& pMatrix);

protected:
    std::unique_ptr<Inferno::GLSLProgram> m_program = nullptr;
};

class MomentumBallRenderer : public BallRenderer {
public: 
    virtual void renderBalls(Inferno::SpriteBatch& spriteBatch,
                             std::vector<Ball>& ball, const glm::mat4& pMatrix) override;
};

class VelocityBallRenderer : public BallRenderer {
public:
    VelocityBallRenderer(int screenWidth, int screenHeight);
	virtual void renderBalls(Inferno::SpriteBatch& spriteBatch,
		std::vector<Ball>& ball, const glm::mat4& pMatrix) override;

private:
    int m_screenWidth, m_screenHeight;
};

class ShadowBallRenderer : public BallRenderer {
public:
  
	virtual void renderBalls(Inferno::SpriteBatch& spriteBatch,
		std::vector<Ball>& ball, const glm::mat4& pMatrix) override;

private:
	int m_screenWidth, m_screenHeight;
};

class TrailBallRenderer : public BallRenderer {
public:
	void renderBalls(Inferno::SpriteBatch& spriteBatch, std::vector<Ball>& balls, const glm::mat4& pMatrix) override;

private:
	struct TrailSegment {
		glm::vec2 position;
		float age;
	};

	std::map<int, std::deque<TrailSegment>> ballTrails; // key: ball's unique ID, value: trail segments
	static const int MAX_TRAIL_LENGTH = 10;
	static const float TRAIL_SEGMENT_LIFETIME;
};

class HaloBallRenderer : public BallRenderer {
public:

	virtual void renderBalls(Inferno::SpriteBatch& spriteBatch,
		std::vector<Ball>& ball, const glm::mat4& pMatrix) override;

private:
	int m_screenWidth, m_screenHeight;
};

