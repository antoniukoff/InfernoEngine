#pragma once
#include <Vladgine/Vertex.h>
#include <Vladgine/SpriteBatch.h>
#include <glm/glm.hpp>

class Light
{
public:
	void draw(Vladgine::SpriteBatch& spriteBatch) {
		glm::vec4 destRect;
		destRect.x = position.x - size / 2.0f;
		destRect.y = position.y - size / 2.0f;
		destRect.z = size;
		destRect.w = size;
		spriteBatch.draw(destRect, glm::vec4(-1.0f, -1.0f, 2.0f, 2.0f),0, 0.0f, color, 0.0f);
	}

	Vladgine::ColorRGB8 color;
	glm::vec2 position;
	float size;
};

