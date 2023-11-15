#pragma once

#include <GLEW/glm/glm.hpp>
#include <GLEW/glm/gtc/matrix_transform.hpp>


namespace Vladgine {

	class Camera2D
	{
	public:

		Camera2D();
		~Camera2D();

		void init(int screenW, int screenH);
		void update();
		// converts screen coordinates to world coordinates
		glm::vec2 converScreenToWorld(glm::vec2 screenCoords);
		// simple aabb test to see if the glyph is within the camera borders. used for camera culling
		bool isBoxInView(const glm::vec2& position, const glm::vec2& domensions);
		void offsetPosition(const glm::vec2& offset) { _position += offset; _needsMatrixUpdate = true; }
		void offsetScale(float cameraOffset) { _scale += cameraOffset; if (_scale <= 0.001f) _scale = 0.001f; _needsMatrixUpdate = true; }
		void setPos(const glm::vec2& newPos) { _position = newPos; _needsMatrixUpdate = true; }
		void setScale(float newScale) { _scale = newScale; _needsMatrixUpdate = true; }
		glm::vec2 getPos() const { return _position; }
		float getScale() const { return _scale; }
		glm::mat4 getCameraMatrix() const { return _cameraMatrix; }
		float getAspectRatio() const { return (float)_screenWidth / (float)_screenHeight; }

	private:
		int _screenWidth, _screenHeight;
		bool _needsMatrixUpdate;
		float _scale;
		glm::vec2 _position;
		glm::mat4 _cameraMatrix;
		glm::mat4 _orthoMatrix;

	};

}
