#include "Camera2D.h"

namespace Vladgine {

	Camera2D::Camera2D() : _position(0.0f, 0.0f), _cameraMatrix(1.0f), _orthoMatrix(1.0f), _scale(1.0f), _needsMatrixUpdate(1),
		_screenWidth(500), _screenHeight(500)
	{
	}

	Camera2D::~Camera2D()
	{
	}

	void Camera2D::init(int screenW, int screenH)
	{
		_screenWidth = screenW;
		_screenHeight = screenH;
		// set ortho matrix to be the size of my screen  coordinates
		// ortho is going to map the ndc with these coordinates
		_orthoMatrix = glm::ortho(0.0f, (float)_screenWidth, 0.0f, (float)_screenHeight);
	}

	void Camera2D::update()
	{
		if (_needsMatrixUpdate) {

			// sets to move everything to the opposite side and 
			//sets the camera origin to the middle of the screen
			glm::vec3 translate(-_position.x + _screenWidth / 2, -_position.y + _screenHeight / 2 , 0.0f);
			//here i set the translation to the camera matrix to be 
			//orthomatrix multiplied by the translate vector
			_cameraMatrix = glm::translate(_orthoMatrix, translate);

			glm::vec3 scale(_scale, _scale, 0.0f);
			_cameraMatrix = glm::scale(glm::mat4(1.0f), scale) * _cameraMatrix;
			_needsMatrixUpdate = false;
		}
	}

	glm::vec2 Camera2D::converScreenToWorld(glm::vec2 screenCoords)
	{
		screenCoords -= glm::vec2(_screenWidth
			/ 2, _screenHeight / 2);
		screenCoords /= _scale;
		screenCoords += _position;
		return screenCoords;
	}

}