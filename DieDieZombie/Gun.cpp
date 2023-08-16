#include "Gun.h"
#include <random>
#include <ctime>
#include <glm/gtx/rotate_vector.hpp>

const float DEG_TO_RAD = 3.14159265359f / 180.0f;
const float RAD_TO_DEG = 180.0f / 3.14159265359f;

Gun::Gun(std::string name, int firerate, int bulletspershot, float spread, float bulletDamage, float bulletSpeed)
{
	_name = name;
	_fireRate = firerate;
	_bulletPerShot = bulletspershot;
	_spread = spread;
	_bulletDamage = bulletDamage;
	_bulletSpeed = bulletSpeed;
	_frameCounter = 0;
}

Gun::~Gun()
{
}

void Gun::update(bool isMouseDown, const glm::vec2& position, const glm::vec2& direction, std::vector<Bullet>& bullets, float deltaTime)
{
	_frameCounter += 1.0f * deltaTime;
	if (_frameCounter >= _fireRate && isMouseDown) {
		fire(direction, position, bullets);
		_frameCounter = 0;
	}
}

void Gun::fire(glm::vec2 direction, const glm::vec2& position, std::vector<Bullet>& bullets)
{
	static std::mt19937 randoEngine(time(nullptr));

	 std::uniform_real_distribution<float> randRotate(-_spread, _spread);

	for (int i = 0; i < _bulletPerShot; i++) {
		bullets.emplace_back(position, glm::rotate(direction, randRotate(randoEngine) * DEG_TO_RAD), _bulletDamage, _bulletSpeed);
	}
}
