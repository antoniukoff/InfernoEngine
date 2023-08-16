#include "Human.h"
#include <string>
#include <random>
#include <ctime>
#include <glm/gtx/rotate_vector.hpp>

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

	_health = 20.0f;

	_color.r = 200;
	_color.g = 0;
	_color.b = 200;
	_color.a = 255;

	_speed = speed;
	_position = position;
	//gets random direction
	_direction = glm::vec2(randDir(randoEngine), randDir(randoEngine));

	//makes sure direction isn't zero
	if (_direction.length() == 0) _direction = glm::vec2(1.0f, 0.0f);

	_direction = glm::normalize(_direction);
}

void Human::update(const std::vector<std::string>& levelData,
	std::vector<Human*>& humans,
	std::vector<Zombie*>& zombie,
	float deltaTime)
{
	static std::mt19937 randoEngine(time(nullptr));

	static std::uniform_real_distribution<float> randRotate(-20.1f, 20.1f);

	_position += _direction * _speed * deltaTime;

	if (_frames == 200) {
		_direction = glm::rotate(_direction, randRotate(randoEngine));
		_frames = 0;
	} else {
		_frames++;
	}

	

		
	if (collideWithLevel(levelData)) {
		_direction = glm::rotate(_direction, randRotate(randoEngine));
	}
}
