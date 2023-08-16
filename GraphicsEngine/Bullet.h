#pragma once
#include<glm/glm.hpp>
#include <Vladgine/SpriteBatch.h>
class Bullet
{
public:
	Bullet(glm::vec2 position, glm::vec2 direction, float speed, int lifeTime);
	~Bullet();

	
	void draw(Vladgine::SpriteBatch& spriteBatch);
	bool update();

private:
	int _lifeTime;
	float _speed;
	glm::vec2 _direction;
	glm::vec2 _position;
};

