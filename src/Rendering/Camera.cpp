#include "Camera.h"
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

const glm::vec3 worldUp{0.0f, 0.0f, 1.0f};

Camera::Camera(float pitch, float yaw, float fov) 
    : position(), pitch(pitch), yaw(yaw), fov(fov), forward(), 
    right(), up(), centrePoint(glm::vec3(0,0,0)), distanceFromTarget(5)
{
    UpdateDirectionVectors();
}

glm::mat4 Camera::GetProjectionMatrix(float width, float height) 
{
    return glm::perspective(glm::radians(fov), width/height, 0.1f, 100.0f);
}

glm::mat4 Camera::GetViewMatrix() 
{
    return glm::lookAt(position, position + forward, up);
}

void Camera::Zoom(float scrollDelta) 
{
    distanceFromTarget -= scrollDelta;
    CalculateMovement(0,0,false);
}

void Camera::CalculateMovement(float mouseXDelta, float mouseYDelta, bool rotate) 
{
    if (rotate) 
    {
        yaw += mouseXDelta / 10;
        pitch -= mouseYDelta / 10;
    }
    else 
    {
        centrePoint -= mouseXDelta / 200 * right;
        centrePoint -= mouseYDelta / 200 * up;
    }
    
    UpdateDirectionVectors();
    
    glm::vec3 positionOffset = forward * -distanceFromTarget;
    position = centrePoint + positionOffset;
}

void Camera::UpdateDirectionVectors() 
{
    float yawRads = glm::radians(yaw);
    float pitchRads = glm::radians(pitch);
    
    forward = glm::normalize(glm::vec3(
        glm::cos(yawRads) * glm::cos(pitchRads),
        glm::sin(yawRads) * glm::cos(pitchRads),
        glm::sin(pitchRads)
    ));
    
    right = glm::normalize(glm::cross(worldUp, forward));
    up = glm::normalize(glm::cross(right, forward));
}
