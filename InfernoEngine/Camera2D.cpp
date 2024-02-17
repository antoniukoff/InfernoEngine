#include "Camera2D.h"

namespace Inferno {

    Camera2D::Camera2D()
        : _position(0.0f, 0.0f),
        _cameraMatrix(1.0f),
        _orthoMatrix(1.0f),
        _scale(1.0f),
        _needsMatrixUpdate(true),
        _screenWidth(500),
        _screenHeight(500)
    {
    }

    Camera2D::~Camera2D()
    {
    }

    void Camera2D::init(int screenW, int screenH)
    {
        _screenWidth = screenW;
        _screenHeight = screenH;

        // Set orthographic matrix to be the size of the screen coordinates
        _orthoMatrix = glm::ortho(0.0f, static_cast<float>(_screenWidth), 0.0f, static_cast<float>(_screenHeight));
    }











    void Camera2D::update()
    {
        if (_needsMatrixUpdate)
        {
            // Calculate translation
            glm::vec3 translate(-_position.x + _screenWidth / 2, -_position.y + _screenHeight / 2, 0.0f);
            _cameraMatrix = glm::translate(_orthoMatrix, translate);

            // Scale the camera matrix
            glm::vec3 scale(_scale, _scale, 0.0f);
            _cameraMatrix = glm::scale(glm::mat4(1.0f), scale) * _cameraMatrix;

            _needsMatrixUpdate = false;
        }
    }






    

    // Converts screen coordinates to world coordinates
    glm::vec2 Camera2D::converScreenToWorld(glm::vec2 screenCoords)
    {
        screenCoords.y = _screenHeight - screenCoords.y;
        screenCoords -= glm::vec2(_screenWidth / 2, _screenHeight / 2);
        screenCoords /= _scale;
        screenCoords += _position;

        return screenCoords;
    }

    // Simple AABB test to see if a box is within the camera borders, used for camera culling
    bool Camera2D::isBoxInView(const glm::vec2& position, const glm::vec2& dimensions)
    {
        glm::vec2 scaledScreenDimension = glm::vec2(_screenWidth, _screenHeight) / _scale;

        // calculate the minimum distance by adding the offset of the objects position and the dimensions
        // to the half of the screen dimensions to add extra padding to the visible area of the camera
        // this is because the objects position is in the center of the object and not the top left corner
        // if the object is at the edge of the screen it will be culled before it is completely
        // out of the screen so by adding the extra padding it will be culled when it is completely out of the screen
        const float MIN_DISTANCE_X = dimensions.x / 2.0f + scaledScreenDimension.x / 2.0f;
        const float MIN_DISTANCE_Y = dimensions.y / 2.0f + scaledScreenDimension.y / 2.0f;

        // Center position of the object in world space
        glm::vec2 centerPos = position + dimensions / 2.0f;
        // Center position of the camera
        glm::vec2 centerCameraPos = _position;
        // Distance between the object's center and the camera's center
        glm::vec2 distVec = centerPos - centerCameraPos;

        float xDepth = MIN_DISTANCE_X - abs(distVec.x);
        float yDepth = MIN_DISTANCE_Y - abs(distVec.y);

        // There was a collision 
        if (xDepth > 0 && yDepth > 0) {
            return true;
        }
        return false;
    }

}


