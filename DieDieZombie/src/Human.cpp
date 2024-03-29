#include "Human.h"
#include <iostream>
#include <string>
#include <random>
#include <ctime>
#include  <GLEW/glm/gtx/rotate_vector.hpp>
#include <InfernoEngine/ResourceManager.h>

constexpr float DEG_TO_RAD = 3.14159265359f / 180.0f;
constexpr float RAD_TO_DEG = 180.0f / 3.14159265359f;

Human::Human(): _frames(0)
{
}

Human::~Human()
{
}

void Human::init(float speed, glm::vec2 position)
{
	static std::mt19937 randoEngine(time(nullptr));
	static std::uniform_real_distribution<float> randDir(-1.0f, 1.0f);
	m_textureID = Inferno::ResourceManager::getTexture("assets/textures/human.png").id;
	m_health = 20.0f;
	m_color = Inferno::ColorRGB8(255, 255, 255, 255);
	m_speed = speed;
	m_position = position;

	//gets random direction
	m_direction = glm::vec2(randDir(randoEngine), randDir(randoEngine));
	
	//makes sure direction isn't zero
	if (m_direction.length() == 0) m_direction = glm::vec2(1.0f, 0.0f);

	m_direction = glm::normalize(m_direction);
}

void Human::update(const std::vector<std::string>& levelData,
	std::vector<Human*>& humans,
	std::vector<Zombie*>& zombie,
	float deltaTime)
{
	static std::mt19937 randoEngine(time(nullptr));
	static std::uniform_real_distribution<float> randRotate(-20.1f, 20.1f);

	m_position += m_direction * m_speed * deltaTime;
	if (_frames == 200) {
		m_direction = glm::rotate(m_direction, randRotate(randoEngine) * DEG_TO_RAD);
		_frames = 0;
	} else {
		_frames++;
	}

	if (collideWithLevel(levelData)) {
		m_direction = glm::rotate(m_direction, randRotate(randoEngine));
	}
}
