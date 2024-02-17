#pragma once
#include "Light.h"
#include <InfernoEngine/GLTexture.h>
#include <vector>

struct LevelData {
	// Boxes
	struct BoxData {
		glm::vec2 position;
		glm::vec2 dimensions;
		Inferno::GLTexture texture;
		Inferno::ColorRGB8 color;
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
		Inferno::ColorRGB8 color;
	};

	PlayerData player;
};