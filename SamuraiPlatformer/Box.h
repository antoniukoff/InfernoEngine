#pragma once
#include <Box2D/Box2D.h>
#include <GLEW/glm/glm.hpp>
#include <InfernoEngine/Vertex.h>
#include <InfernoEngine/SpriteBatch.h>
#include <InfernoEngine/GLTexture.h>

class Box
{
public:
	Box();
	~Box();

	void init(b2World* world, const glm::vec2& position, const glm::vec2& dimensions, Vladgine::GLTexture texture,
		Vladgine::ColorRGB8 color, bool fixedRotation, bool isDynamic,
		float angle = 0.0f, glm::vec4 uvRrct = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));

	void destroy(b2World* world);

	void draw(Vladgine::SpriteBatch& spriteBatch);

	bool isDynamic() const { return m_body->GetType() == b2_dynamicBody; }

	// Test if a point is inside the box
	bool testPoint(float x, float y) const { return m_fixture->TestPoint(b2Vec2(x, y)); }

	b2Body*                    getBody() const           { return m_body; }
	b2Fixture*                 getFixture() const        { return m_fixture; }
	const glm::vec2&           getDimensions() const     { return m_dimensions; }
	glm::vec2                  getPosition() const       { return glm::vec2(m_body->GetPosition().x, m_body->GetPosition().y); }
	glm::vec4                  getUvRect() const         { return m_uvRect; }
	const Vladgine::ColorRGB8& getColor() const       { return m_color; }
	float                      getAngle() const          { return m_body->GetAngle(); }
	const Vladgine::GLTexture& getTexture() const     { return m_texture; }
	const bool&                getFixedRotation() const { return m_fixedRotation; }
	const bool&                getIsDynamic() const { return m_isDynamic; }



private:
	glm::vec4 m_uvRect;
	b2Body* m_body = nullptr;
	b2Fixture* m_fixture = nullptr;
	glm::vec2 m_dimensions;
	Vladgine::ColorRGB8 m_color;
	Vladgine::GLTexture m_texture;
	bool m_fixedRotation;
	bool m_isDynamic;
};

