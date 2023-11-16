#include "ParticleBatch2D.h"

Inferno::ParticleBatch2D::ParticleBatch2D()
{
	//empty
}

Inferno::ParticleBatch2D::~ParticleBatch2D()
{
	delete[] m_particles;
}

void Inferno::ParticleBatch2D::init(int maxParticles, float decayRate, 
	GLTexture texture, std::function<void(Particle2D&, float)> updateFunc /* = defaultParticleUpdate*/)
{
	m_maxPArticles = maxParticles;
	m_particles = new Particle2D[maxParticles];
	m_decayRate = decayRate;
	m_texture = texture;
	m_updateFunc = updateFunc;
}

void Inferno::ParticleBatch2D::update(float deltaTime)
{
	for (int i = 0; i < m_maxPArticles; i++) {
		if (m_particles[i].life > 0.0f) {
			m_updateFunc(m_particles[i], deltaTime);
			m_particles[i].life -= m_decayRate * deltaTime;
		}
	}
}

void Inferno::ParticleBatch2D::draw(SpriteBatch* spriteBatch)
{
	glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
	for (int i = 0; i < m_maxPArticles; i++) {
		auto& p = m_particles[i];
		if (m_particles[i].life > 0.0f) {
			glm::vec4 destRect(p.position.x, p.position.y, p.width, p.width);
			spriteBatch->draw(destRect, uvRect, m_texture.id, 0.0f, p.color);
		}
	}
}

void Inferno::ParticleBatch2D::addParticle(const glm::vec2& position, 
											const glm::vec2& velocity, 
											const ColorRGB8& color, float width)
{
	int particleIndex = findFreeParticle();
	auto& p = m_particles[particleIndex];
	p.life = 1.0f;
	p.position = position;
	p.velocity = velocity;
	p.color = color;
	p.width = width;
}

int Inferno::ParticleBatch2D::findFreeParticle()
{
	for (int i = m_lastFreeParticle; i < m_maxPArticles; i++) {
		if (m_particles[i].life <= 0.0f) {
			m_lastFreeParticle = i;
			return i;
		}
	}
	for (int i = 0; i < m_lastFreeParticle; i++) {
		if (m_particles[i].life <= 0.0f) {
			m_lastFreeParticle = i;
			return i;
		}
	}
	return 0;
}

