#pragma once
#include <glm/glm.hpp>
#include <Vladgine/SpriteBatch.h>
#include <string>
#include <Vladgine/Vertex.h>

const float AGENT_WIDTH = 60.0f;
const float AGENT_RADIUS = (float)AGENT_WIDTH / 2;

class Zombie;
class Human;


class Agent
{
public:
	Agent();
	virtual ~Agent();

	virtual void update(const std::vector<std::string>& levelData,
						      std::vector<Human*>& humans,
						      std::vector<Zombie*>& zombie,
							  float deltaTime) = 0;

	bool collideWithLevel(const std::vector<std::string>& levelData);

	bool collideWithAgent(Agent* agent);

	void draw(Vladgine::SpriteBatch& _spriteBatch);

	bool applyDamage(float Damage);

	glm::vec2 getPosition() const { return m_position; }

protected:
	void checkTilePosition(const std::vector<std::string>& levelData,
								 std::vector<glm::vec2>& collideTilePositions,
								 float x, float y);

	void collideWithTile(glm::vec2 tilePos);

	glm::vec2 m_position;
	float m_speed;
	Vladgine::ColorRGB8 m_color;
	GLuint m_textureID;
	glm::vec2 m_direction = glm::vec2(1.0f, 0.0f);
	float m_health;
};

