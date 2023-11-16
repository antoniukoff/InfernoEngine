#include "ParticleEngine2D.h"
#include "SpriteBatch.h"
#include "ParticleBatch2D.h"

Inferno::ParticleEngine2D::ParticleEngine2D()
{
}

Inferno::ParticleEngine2D::~ParticleEngine2D()
{
	for (auto& b : m_partileBatches) {
		delete b;
	}
}

void Inferno::ParticleEngine2D::addParticleBatch(ParticleBatch2D* particleBatch)
{
	m_partileBatches.push_back(particleBatch);
}

void Inferno::ParticleEngine2D::update(float deltaTime)
{
	for (auto& b : m_partileBatches) {
		b->update(deltaTime);
	}
}

void Inferno::ParticleEngine2D::draw(SpriteBatch* spriteBatch)
{
	for (auto& b : m_partileBatches) {
		spriteBatch->begin();
		b->draw(spriteBatch);
		spriteBatch->end();
		spriteBatch->renderBatch();
	}
}
