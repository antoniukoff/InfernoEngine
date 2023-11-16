#include "Zombie.h"
#include "Human.h"
#include <InfernoEngine/ResourceManager.h>
Zombie::Zombie()
{
}

Zombie::~Zombie()
{
}

void Zombie::init(float speed, glm::vec2 position)
{
	m_speed = speed;
	m_position = position;

	m_health = 150.0f;
	m_color = Inferno::ColorRGB8(255, 255, 255, 255);
	m_textureID = Inferno::ResourceManager::getTexture("Textures/zombie.png").id;

}

void Zombie::update(const std::vector<std::string>& levelData,
	std::vector<Human*>& humans,
	std::vector<Zombie*>& zombie,
	float deltaTime)
{
	Human* closestHuman = getNearestHuman(humans);
	if (closestHuman != nullptr) {
		m_direction = glm::normalize(closestHuman->getPosition() - m_position);
		m_position += m_direction * m_speed * deltaTime;
	}

	collideWithLevel(levelData);
}

Human* Zombie::getNearestHuman(std::vector<Human*>& humans)
{
	Human* closestHUman = nullptr;
	 float smallestDistance = 9999999.0f;
	for (int i = 0; i < humans.size(); i++) {
		glm::vec2 distVec = humans[i]->getPosition() - m_position;
		float distance = glm::length(distVec);

		if (distance < smallestDistance) {
			smallestDistance = distance;
			closestHUman = humans[i];
		}
	}
	return closestHUman;
}
