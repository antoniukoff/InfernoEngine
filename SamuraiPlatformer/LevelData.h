#pragma once
#include "glm/glm.hpp"
#include "Light.h"
#include <Vladgine/Vertex.h>
#include <Vladgine/GLTexture.h>
#include <vector>

struct LevelData {
	// Boxes
	struct BoxData {
		glm::vec2 position;
		glm::vec2 dimensions;
		Vladgine::GLTexture texture;
		Vladgine::ColorRGB8 color;
		bool fixedRotation;
		bool isDynamic;
		float angle;
		glm::vec4 uvRect;
	};
	std::vector<BoxData> boxes;

	std::vector<Light> light;

	// Player
	struct PlayerData {
		glm::vec2 position;
		glm::vec2 drawDims;
		glm::vec2 collisionDims;
		Vladgine::ColorRGB8 color;
	};

	PlayerData player;
};