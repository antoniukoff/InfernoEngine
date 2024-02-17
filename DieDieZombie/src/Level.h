#pragma once
#include <string>
#include <vector>
#include <InfernoEngine/SpriteBatch.h>

const int TILE_WIDTH = 64;

class Level
{
public:
	//load the level
	Level(const std::string& fileName);
	~Level();


	void draw();


	//Getters
	int getWidth() const {
		if (_levelData.empty()) return 0;
		return _levelData[0].size();
	}

	int getHeight() const { return _levelData.size(); }
	const std::vector<std::string>& getLevelData() const { return _levelData; }
	glm::vec2 getStartPlayerPos() const { return _playerStartPos; }
	const std::vector<glm::vec2>& getZombieStartPos() const { return _zombieStartPos; }
	int getNumHumans() const { return _numHumans; }

private:
	std::vector<std::string> _levelData;
	int _numHumans;
	Inferno::SpriteBatch _spriteBatch;

	glm::vec2 _playerStartPos;
	std::vector<glm::vec2> _zombieStartPos;
};
