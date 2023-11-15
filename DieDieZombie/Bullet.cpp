#include "Bullet.h"
#include <InfernoEngine/ResourceManager.h>
#include "Agent.h"
#include "Human.h"
#include "Zombie.h"
#include <SDL/SDL_mixer.h>
#include "Level.h"

Bullet::Bullet(glm::vec2 position, glm::vec2 direction, float damage, float speed)
{
	m_position = position;
	m_direction = direction;
	m_damage = damage; 
	m_speed = speed;
}

Bullet::~Bullet()
{
}

bool Bullet::update(const std::vector<std::string>& levelData, float deltaTime)
{
	m_position += m_direction * m_speed * deltaTime;

	return collideWithWorld(levelData);

}

void Bullet::draw(Vladgine::SpriteBatch& spriteBatch)
{
	glm::vec4 destRect(m_position.x + BULLET_RADIUS, m_position.y + BULLET_RADIUS, BULLET_RADIUS * 2, BULLET_RADIUS * 2);
	
	const glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
	Vladgine::ColorRGB8 color;
	color.r = 255;
	color.g = 255;
	color.b = 255;
	color.a = 255;

	spriteBatch.draw(destRect, uvRect, Vladgine::ResourceManager::getTexture("Textures/PNG/Bullet.png").id, 0.0f, color); 
}

bool Bullet::collideWithAgent(Agent* agent)
{
	const float MIN_DISTANCE = AGENT_RADIUS + BULLET_RADIUS;

	glm::vec2 centerPosA = m_position;
	glm::vec2 centerPosB = agent->getPosition() + glm::vec2(AGENT_RADIUS);

	glm::vec2 distVec = centerPosA - centerPosB;

	float distance = glm::length(distVec);

	float collisionDepth = MIN_DISTANCE - distance;

	if (collisionDepth > 0) {

		return true;
	}

	return false;
}

bool Bullet::collideWithWorld(const std::vector<std::string>& levelData)
{
	glm::ivec2 gridPosition;
	gridPosition.x = floor(m_position.x / (float)TILE_WIDTH);
	gridPosition.y = floor(m_position.y / (float)TILE_WIDTH);

	if (gridPosition.x < 0 || gridPosition.x >= levelData[0].length() ||
		gridPosition.y < 0 || gridPosition.y >= levelData.size()) {
		return false;
	}
	return (levelData[gridPosition.y][gridPosition.x] != '.');
}



