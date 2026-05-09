#pragma once

#include <Eigen/Core>

class Camera 
{
    public:
        Camera(float pitch = 0.0f, float yaw = 90.0f, float fov = 60.0f); 
        
        void Translate(float mouseXDelta, float mouseYDelta);
        void Rotate(float mouseXDelta, float mouseYDelta);
        void Zoom(float scrollDelta);
        
        Eigen::Matrix4f GetProjectionMatrix(float width, float height);
        Eigen::Matrix4f GetViewMatrix(); 
        Eigen::Matrix4f GetOtrhoMatrix(float width, float height);
        
        Eigen::Vector3f position; 
        
        Eigen::Vector3f forward;
        Eigen::Vector3f right;
        Eigen::Vector3f up;
    private:
        void Reposition(); 

        Eigen::Vector3f centrePoint;
        float distanceFromTarget;
        
        float nearClip;
        float farClip;
        float fov;

        float pitch;
        float yaw;
    };