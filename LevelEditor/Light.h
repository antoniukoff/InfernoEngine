#pragma once
#include <InfernoEngine/Vertex.h>
#include <InfernoEngine/SpriteBatch.h>
#include <GLEW/glm/glm.hpp>

class Light
{
public:
	void draw(Inferno::SpriteBatch& spriteBatch) {
		glm::vec4 destRect;
		destRect.x = position.x - size / 2.0f;
		destRect.y = position.y - size / 2.0f;
		destRect.z = size;
		destRect.w = size;
		spriteBatch.draw(destRect, glm::vec4(-1.0f, -1.0f, 2.0f, 2.0f),0, 0.0f, color, 0.0f);
	}

	Inferno::ColorRGB8 color;
	glm::vec2 position;
	float size;
};

