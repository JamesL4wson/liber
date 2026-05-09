#include <Eigen/Geometry>
#include <cmath>

#include "Camera.h"

using namespace Eigen;

const float DEG_TO_RAD = 0.01745329251;
const Vector3f worldUp{0.0f, 0.0f, 1.0f};

Camera::Camera(float pitch, float yaw, float fov) 
    : position(), pitch(pitch), yaw(yaw), fov(fov), nearClip(0.1f),
    farClip(100.0f), forward(), right(), up(), 
    centrePoint(Vector3f(0,0,0)), distanceFromTarget(5)
{
    Reposition();
}

void Camera::Zoom(float scrollDelta) 
{
    distanceFromTarget -= scrollDelta;
    Reposition();
}

void Camera::Translate(float mouseXDelta, float mouseYDelta) 
{
    centrePoint -= mouseXDelta / 200 * right;
    centrePoint += mouseYDelta / 200 * up;
    Reposition();
}

void Camera::Rotate(float mouseXDelta, float mouseYDelta)
{
    yaw += mouseXDelta / 10;
    pitch += mouseYDelta / 10;
    Reposition();
}

void Camera::Reposition() 
{
    float yawRads = yaw * DEG_TO_RAD;
    float pitchRads = pitch * DEG_TO_RAD;
    
    forward = (Vector3f(
        std::cos(yawRads) * std::cos(pitchRads),
        std::sin(yawRads) * std::cos(pitchRads),
        std::sin(pitchRads)).normalized());
    
    right = (worldUp.cross(forward)).normalized();
    up = right.cross(forward);

    position = centrePoint - forward * distanceFromTarget;
}

Matrix4f Camera::GetProjectionMatrix(float width, float height) 
{
    /*
    Perspective projection matrix for "symmetrical view frustum 
    with field of view, aspect ratio, near and far clip planes".
    As described at:
    
    https://cseweb.ucsd.edu/classes/wi20/cse167-a/lec4.pdf pg22
    */
   
    float a = 1 / (std::tan(DEG_TO_RAD * fov/2));
    float b = height/width * a;
    float c = (nearClip + farClip) / (nearClip - farClip);
    float d = (2 * nearClip * farClip) / (nearClip - farClip);
   
    Matrix4f perspective;
    perspective <<  b,  0,  0, 0,
                    0, -a,  0, 0, 
                    0,  0,  c, d, 
                    0,  0, -1, 0;

    return perspective;
}

Matrix4f Camera::GetOtrhoMatrix(float width, float height) 
{
    /*
    Orthographic projection matrix defined by screen dimensions.

    As described at:
    
    https://en.wikipedia.org/wiki/Orthographic_projection
    */
   
    float r = -100;
    float l = 100;
    float b = -50;
    float t = 50;
   
    Matrix4f perspective;
    perspective <<  2*(r-l), 0,        0,                    -(r+l) / (r-l),
                    0,       2*(t-b),  0,                    -(t+b) / (t-b), 
                    0,       0,       -2*(farClip-nearClip), -(farClip+nearClip)/(farClip-nearClip), 
                    0,       0,        0,                     1;

    return perspective;
}

Matrix4f Camera::GetViewMatrix() 
{
    /*
    "Look at" matrix for alligning the camera view frame and direction
    correctly. As described at:

    https://stackoverflow.com/questions/349050/calculating-a-lookat-matrix
    */

    Matrix4f view;
    view <<   right[0],    right[1],    right[2],  -right.dot(position),
             -up[0],      -up[1],      -up[2],      up.dot(position),
             -forward[0], -forward[1], -forward[2], forward.dot(position),
              0,           0,           0,          1;

    return view;
}
