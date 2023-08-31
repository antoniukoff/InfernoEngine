#pragma once

#include <Vladgine/InputManager.h>
#include <Vladgine/SpriteBatch.h>
#include <Vladgine/GLTexture.h>
#include <Vladgine/TileSheet.h>
#include "Capsule.h"

enum class MoveState { STANDING, RUNNING, PUNCHING, IN_AIR };

class Player {
public:

	void init(b2World* world, const glm::vec2& position, const glm::vec2& drawDims, const glm::vec2& collisionDims, Vladgine::ColorRGB8 color);

	void destroy(b2World* world);

	void draw(Vladgine::SpriteBatch& spriteBatch);
	void drawDebug(Vladgine::DebugRenderer& debugRenderer);

	void update(Vladgine::InputManager& inputManager);

	const Capsule& getCapsule() const { return m_capsule; }

	glm::vec2& getPosition() const {
	
		glm::vec2 rv;
		rv.x = m_capsule.getBody()->GetPosition().x;
		rv.y = m_capsule.getBody()->GetPosition().y;

		return rv;
 }

private:
	MoveState m_moveState = MoveState::STANDING;
	Vladgine::TileSheet m_texture;
	Vladgine::ColorRGB8 m_color;
	Capsule m_capsule;
	float m_animTime = 0;
	glm::vec2 m_drawDims;
	bool m_onGround = false;
	bool m_punching = false;
	int m_direction = 1;
};