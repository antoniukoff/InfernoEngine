#pragma once
#include <Box2D/Box2D.h>
#include <glm/glm.hpp>
#include <Vladgine/Vertex.h>
#include <Vladgine/SpriteBatch.h>
#include <Vladgine/GLTexture.h>

class Box
{
public:
	Box();
	~Box();

	void init(b2World* world, const glm::vec2& position, const glm::vec2& dimensions, Vladgine::GLTexture texture,
			  Vladgine::ColorRGB8 color, bool fixedRotation, glm::vec4 uvRrct = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));

	void draw(Vladgine::SpriteBatch& spriteBatch);

	b2Body* getBody() const { return m_body; }
	b2Fixture* getFixture() const { return m_fixture; }
	const glm::vec2& getDimensions() const { return m_dimensions; }
	const Vladgine::ColorRGB8& getColor() const { return m_color; }

private:
	glm::vec4 m_uvRect;
	b2Body* m_body = nullptr;
	b2Fixture* m_fixture = nullptr;
	glm::vec2 m_dimensions;
	Vladgine::ColorRGB8 m_color;
	Vladgine::GLTexture m_texture;
};

