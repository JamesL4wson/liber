#pragma once

#include <glm/glm.hpp>

class Camera 
{
    public:
        Camera(float pitch = 0.0f, float yaw = 90.0f, float fov = 60.0f); 
        
        void UpdateDirectionVectors(); 
        glm::mat4 GetProjectionMatrix(float width, float height);
        glm::mat4 GetViewMatrix(); 
        
        void CalculateMovement(float mouseXDelta, float mouseYDelta, bool translate);
        void Zoom(float scrollDelta);
        
        glm::vec3 position; 
        
        float fov;
        
    private:
        glm::vec3 forward;
        glm::vec3 right;
        glm::vec3 up;
    
        glm::vec3 centrePoint;
        float distanceFromTarget;

        float pitch;
        float yaw;
    };