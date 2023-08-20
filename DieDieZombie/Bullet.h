#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include <Vladgine/SpriteBatch.h>

class Human;
class Zombie;
class Agent;

const int BULLET_RADIUS = 5;

class Bullet
{
public:
	Bullet(glm::vec2 position, glm::vec2 direction, float damage, float speed);
	~Bullet();

	//when update returns true, delete bullet
	bool update(const std::vector<std::string>& levelData, float deltaTime);

	void draw(Vladgine::SpriteBatch& spriteBatch);

	bool collideWithAgent(Agent* agent);

	float getDamage() { return m_damage; }

	glm::vec2 getPosition() const { return m_position; }

private:
	bool collideWithWorld(const std::vector<std::string>& levelData);

	float m_damage;
	glm::vec2 m_position;
	glm::vec2 m_direction;
	float m_speed;
};

