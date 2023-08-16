#pragma once
#include "Human.h"
#include <Vladgine/Camera2D.h>
#include <Vladgine/InputManager.h>
#include <string>
#include "Bullet.h"

class Gun;


class Player : public Human
{
public: 
	Player();
	~Player();
	void init(float speed, glm::vec2 position, Vladgine::InputManager* inputManager, 
		Vladgine::Camera2D* camera, std::vector<Bullet>* bullets);

	void addGun(Gun* gun);

	void update(const std::vector<std::string>& levelData,
		std::vector<Human*>& humans,
		std::vector<Zombie*>& zombie,
		float deltaTime);
private:

	Vladgine::InputManager* _inputManager;
	std::vector<Gun*> _guns;
	int _currentGunIndex;
	Vladgine::Camera2D* _camera;
	std::vector<Bullet>* _bullets;
};

