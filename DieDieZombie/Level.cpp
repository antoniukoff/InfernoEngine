#include "Level.h"
#include "fstream"
#include <iostream>
#include <Vladgine/Errors.h>
#include <Vladgine/ResourceManager.h>

Level::Level(const std::string& fileName)
{
	std::ifstream file;
	file.open(fileName);

	//error checking
	if (file.fail()) {
		Vladgine::fatalError("failed to open " + fileName);
	}

	std::string temp;
	file >> temp >> _numHumans;

	std::getline(file, temp);
	while (std::getline(file, temp)) {
		_levelData.push_back(temp);
	}
	_spriteBatch.init();
	_spriteBatch.begin();

	glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);

	Vladgine::ColorRGB8 whiteColor;
	whiteColor.r = 255;
	whiteColor.g = 255;
	whiteColor.b = 255;
	whiteColor.a = 255;



	for (int y = 0; y < _levelData.size(); y++) {
		for (int x = 0; x < _levelData[y].size(); x++) {
			//grab the tile
			char tile = _levelData[y][x];

			// get dest rect
			glm::vec4 destRect(x * TILE_WIDTH, y * TILE_WIDTH, TILE_WIDTH, TILE_WIDTH);

			//processing the tile
			switch (tile) {
			case 'R':
				_spriteBatch.draw(destRect,
					uvRect,
					Vladgine::ResourceManager::getTexture("Textures/Walls/brick_wall.png").id,
					0.0f, whiteColor);
				break;
			case 'G':
				_spriteBatch.draw(destRect,
					uvRect,
					Vladgine::ResourceManager::getTexture("Textures/Walls/brick_wall_night.png").id,
					0.0f, whiteColor);
				break;
			case 'B':
			case 'L':
				_spriteBatch.draw(destRect,
					uvRect,
					Vladgine::ResourceManager::getTexture("Textures/Walls/brick_wall_sunny.png").id,
					0.0f, whiteColor);
				break;
			case '@':
				_levelData[y][x] = '.';
				_playerStartPos.x = x * TILE_WIDTH;
				_playerStartPos.y = y * TILE_WIDTH;
				break;
			case 'Z':
				_levelData[y][x] = '.';
				_zombieStartPos.emplace_back(x * TILE_WIDTH, y * TILE_WIDTH);
				break;
			case '.':
				break;
			default:
				printf("Unexpected symbol %c at (%d, %d)", tile, x, y);
				break;
			}
		}
	}

	_spriteBatch.end();
	
}

Level::~Level()
{
}

void Level::draw()
{
	_spriteBatch.renderBatch();
}
