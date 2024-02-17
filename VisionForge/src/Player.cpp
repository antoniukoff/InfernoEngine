#include "Player.h"
#include <InfernoEngine/ResourceManager.h>
#include <SDL/SDL.h>
#include <iostream>

void Player::init(b2World* world, const glm::vec2& position, const glm::vec2& drawDims, const glm::vec2& collisionDims, Inferno::ColorRGB8 color)
{
	Inferno::GLTexture texture = Inferno::ResourceManager::getTexture("assets/Textures/blue_ninja.png");
	m_color = color;
	m_drawDims = drawDims;
	m_capsule.init(world, position, collisionDims, 1.0f, 0.1f, true);
	m_texture.init(texture, glm::ivec2(10, 2));
	m_collisionDims = collisionDims;
}

void Player::destroy(b2World* world)
{
	m_capsule.destroy(world);
}

void Player::draw(Inferno::SpriteBatch& spriteBatch)
{

	glm::vec4 destRect;
	b2Body* body = m_capsule.getBody();
	destRect.x = body->GetPosition().x - m_drawDims.x / 2.0f;
	destRect.y = body->GetPosition().y - m_capsule.getDimensions().y / 2.0f;
	destRect.z = m_drawDims.x;
	destRect.w = m_drawDims.y;

	int tileIndex;
	int numTiles;

	float animSpeed = 0.2f;
	glm::vec2 velocity;
	velocity.x = body->GetLinearVelocity().x;
	velocity.y = body->GetLinearVelocity().y;

	//calculate animation
	if (m_onGround) {
		numTiles = 1;
		tileIndex = 0;
		if (m_punching) {
			numTiles = 4; 
			tileIndex = 1;
			if (m_moveState != MoveState::PUNCHING) {
				m_moveState = MoveState::PUNCHING;
				m_animTime = 0.0f;
			}

		} else if (abs(velocity.x) > 7.0f && ((velocity.x > 0 && m_direction > 0) || (velocity.x < 0 && m_direction < 0))) {
			numTiles = 6;
			tileIndex = 10;
			animSpeed = abs(velocity.x * 0.7f) * 0.03f;
			if (m_moveState != MoveState::RUNNING) {
				m_moveState = MoveState::RUNNING;
				m_animTime = 0;
			}
		}
		else {
			//standing
			numTiles = 1;
			tileIndex = 0;
			m_moveState = MoveState::STANDING;
		}
	}
	else {

		if (m_punching) {
			numTiles = 1;
			tileIndex = 18;
			animSpeed *= 0.25;
			if (m_moveState != MoveState::PUNCHING) {
				m_moveState = MoveState::PUNCHING;
				m_animTime = 0;
			}

		} else if (abs(velocity.x) > 10.0f) {
			numTiles = 1;
			tileIndex = 10;
		}
		// in the air
		else if (velocity.y <= 0.0f) {
			//falling
			numTiles = 1;
			tileIndex = 17;
			m_moveState = MoveState::IN_AIR;
		}
		else {
			//rising
			numTiles = 1;
			tileIndex = 16;
			m_moveState = MoveState::IN_AIR;
		}
	}

	m_animTime += animSpeed;
	
	// check for punch end
	if (m_animTime > numTiles) {
		m_punching = false;
	}

	tileIndex = tileIndex + (int)m_animTime % numTiles;

	glm::vec4 uvRect = m_texture.getUVs(tileIndex);

	if (m_direction == -1) {
		//offsets texture animation frame by the next frame and flips the direction the uv
		uvRect.x += 1.0f / m_texture.dims.x;
		uvRect.z *= -1;
	}

	spriteBatch.draw(destRect, uvRect, m_texture.texture.id, 0.0f, m_color, body->GetAngle());
}

void Player::drawDebug(Inferno::DebugRenderer& debugRenderer)
{
	m_capsule.drawDebug(debugRenderer);	  
}

void Player::update(Inferno::InputManager& inputManager)
{
	b2Body* body = m_capsule.getBody();
	if (inputManager.isKeyDown(SDLK_a)) {

		m_capsule.getBody()->ApplyForceToCenter(b2Vec2(-100.0f, 0.0f), true);
		m_direction = -1;
	}
	else if (inputManager.isKeyDown(SDLK_d)) {

		m_capsule.getBody()->ApplyForceToCenter(b2Vec2(100.0f, 0.0f), true);
		m_direction = 1;
	}
	else {
		body->SetLinearVelocity(b2Vec2(body->GetLinearVelocity().x * 0.95f, body->GetLinearVelocity().y));
	}
	
	float maxSpeed = 10.0f;
	if (body->GetLinearVelocity().x < -maxSpeed) {
		body->SetLinearVelocity(b2Vec2(-maxSpeed, body->GetLinearVelocity().y));
	}
	else if (body->GetLinearVelocity().x > maxSpeed) {
		body->SetLinearVelocity(b2Vec2(maxSpeed, body->GetLinearVelocity().y));
	}

	if (inputManager.isKeyPressed(SDLK_g)) {
		m_punching = true;
	}

	//loop through all contact points
	//loop through dall contact points
	m_onGround = false;
	for (b2ContactEdge* ce = body->GetContactList(); ce != nullptr; ce = ce->next) {
		b2Contact* c = ce->contact;
		if (c->IsTouching()) {
			b2WorldManifold manifold;
			c->GetWorldManifold(&manifold);
			// check if points are below us

			bool below = false;
			for (int i = 0; i < b2_maxManifoldPoints; i++) {
				if (manifold.points[i].y < body->GetPosition().y - m_capsule.getDimensions().y / 2.0f + 0.01f) {
					below = true;
					break;
				}

			}
			if (below) {
				m_onGround = true;

				// we can jump
				if (inputManager.isKeyPressed(SDLK_SPACE)) {
					body->ApplyLinearImpulse(b2Vec2(0.0f, 40.0f), b2Vec2(0.0f, 0.0f), true);
					break;
				}
			}
		}
	}
}
