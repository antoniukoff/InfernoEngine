#pragma once
#include "Agent.h"
#include <string>
	 
class Zombie : public Agent
{
public:
	Zombie();
	~Zombie();

	void init(float speed, glm::vec2 position);

	virtual void update(const std::vector<std::string>& levelData,
		std::vector<Human*>& humans,
		std::vector<Zombie*>& zombie,
		float deltaTime) override;

private:
	Human* getNearestHuman(std::vector<Human*>& humans);
};

