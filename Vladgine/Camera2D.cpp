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
			glm::vec3 translate(-_position.x + _screenWidth / 2, -_position.y + _screenHeight / 2, 0.0f);
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
		// invert y dir
		screenCoords.y = _screenHeight - screenCoords.y;

		screenCoords -= glm::vec2(_screenWidth/ 2, _screenHeight / 2);
		screenCoords /= _scale;
		screenCoords += _position;
		return screenCoords;
	}


	//simple aabb test to see if the glyph is within the camera borders. used for camera culling
	bool Camera2D::isBoxInView(const glm::vec2& position, const glm::vec2& domensions)
	{
		glm::vec2 scaledScreenedDimension = glm::vec2(_screenWidth, _screenHeight) / (_scale);

		const float MIN_DISTANCE_X = domensions.x / 2.0f + scaledScreenedDimension.x / 2.0f;
		const float MIN_DISTANCE_Y = domensions.y / 2.0f + scaledScreenedDimension.y / 2.0f;

		//center position of the parameters
		glm::vec2 centerPos = position + domensions / 2.0f;
		// center position of the camera
		glm::vec2 centerCameraPos = _position;
		//vector from the input to the camera
		glm::vec2 distVec = centerPos - centerCameraPos;

		float xDepth = MIN_DISTANCE_X - abs(distVec.x);
		float yDepth = MIN_DISTANCE_Y - abs(distVec.y);


		// there was a collision 
		if (xDepth > 0 && yDepth > 0){
			return true;
		}
		
		return false;
	}

}