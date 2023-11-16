#include "InputManager.h"

namespace Inferno {

	InputManager::InputManager() : _mouseCoords(0.0f, 0.0f)
	{
	}
	InputManager::~InputManager()
	{
	}

	void InputManager::update()
	{
		//loop through key map and copy if over to previous keymap
		for (auto& it : _keyMap)
		{
			_previousKeyMap[it.first] = it.second;
		}
	}

	void InputManager::pressKey(unsigned int keyID)
	{
		_keyMap[keyID] = true;
	}

	void InputManager::releaseKey(unsigned int keyID)
	{
		_keyMap[keyID] = false;
	}

	bool InputManager::isKeyDown(unsigned int keyID)
	{
		auto it = _keyMap.find(keyID);
		if (it != _keyMap.end()) {
			return it->second;
		}
		else {
			return false;
		}
	}

	bool InputManager::isKeyPressed(unsigned int keyID)
	{
		//check if it was pressed this frame and wasn't pressed last frame
		if (isKeyDown(keyID) == true && wasKeyDown(keyID) == false) { 
			return true;
		}
		return false;
	}

	void InputManager::setMouseCoords(float x, float y)
	{
		_mouseCoords.x = x;
		_mouseCoords.y = y;
	}
	
	bool InputManager::wasKeyDown(unsigned int keyID)
	{
		auto it = _previousKeyMap.find(keyID);
		if (it != _previousKeyMap.end()) {
			return it->second;
		}
		else {
			return false;
		}

	}

}