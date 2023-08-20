#include "Player.h"

#include <SDL.h>
#include <Vladgine/ResourceManager.h>

#include "Gun.h"


Player::Player() :_currentGunIndex(-1)
{
}

Player::~Player()
{
}

void Player::init(float speed, glm::vec2 position, Vladgine::InputManager* inputManager, Vladgine::Camera2D* camera, std::vector<Bullet>* bullets)
{
	m_speed = speed;
	m_position = position;
	m_color = Vladgine::ColorRGB8(255, 255, 255, 255);
	_inputManager = inputManager;
	_camera = camera;
	_bullets = bullets;

	m_textureID = Vladgine::ResourceManager::getTexture("Textures/player.png").id;
}

void Player::addGun(Gun* gun)
{
	_guns.push_back(gun);

	//if no gun equipped, equip gun
	if (_currentGunIndex == -1) {
		_currentGunIndex = 0;
	}

}

void Player::update(const std::vector<std::string>& levelData,
	std::vector<Human*>& humans,
	std::vector<Zombie*>& zombie,
	float deltaTime)
{

	if (_inputManager->isKeyDown(SDLK_w)) {
		m_position.y += m_speed* deltaTime;
	}
	else if (_inputManager->isKeyDown(SDLK_s)) {
		m_position.y -= m_speed* deltaTime;
	}
	if (_inputManager->isKeyDown(SDLK_d)) {
		m_position.x += m_speed * deltaTime;
	}
	else if (_inputManager->isKeyDown(SDLK_a)) {
		m_position.x -= m_speed * deltaTime;
	}

	if (_inputManager->isKeyDown(SDLK_1) && _guns.size() >= 0) {
		_currentGunIndex = 0;
	}
	if (_inputManager->isKeyDown(SDLK_2) && _guns.size() >= 1) {
		_currentGunIndex = 1;
	}
	if (_inputManager->isKeyDown(SDLK_3) && _guns.size() >= 2) {
		_currentGunIndex = 2;
	}

	glm::vec2 mouseCoords = _inputManager->getMouseCoords();

	mouseCoords = _camera->converScreenToWorld(mouseCoords);

	glm::vec2 centerPosition = m_position + glm::vec2(AGENT_RADIUS);

	m_direction = glm::normalize(mouseCoords - centerPosition);

	if (_currentGunIndex != -1) {
		
		_guns[_currentGunIndex]->update(_inputManager->isKeyDown(SDL_BUTTON_LEFT), centerPosition, m_direction, *_bullets, deltaTime);
		
	}

	collideWithLevel(levelData);
}