/*
  ==============================================================================

    Camera.cpp
    Created: 28 Sep 2018 10:59:05am
    Author:  Bowen Sun

  ==============================================================================
*/

#include "Camera.h"
#include "../Globals.h"

void Camera::Setup()
{
    w = (eye - lookAt).normalised();
    u = (up ^ w).normalised();
    v = (w ^ u).normalised();
    if (eye.x == lookAt.x && eye.z == lookAt.z)
    {
        if (eye.y > lookAt.y)
        {
            u = Vec3D(0.0, 0.0, 1.0);
            v = Vec3D(1.0, 0.0, 0.0);
            w = Vec3D(0.0, 1.0, 0.0);
        }
        else if (eye.y < lookAt.y)
        {
            u = Vec3D(1.0, 0.0, 0.0);
            v = Vec3D(0.0, 0.0, 1.0);
            w = Vec3D(0.0, -1.0, 0.0);
        }
    }
}

Camera::Camera(Point3D eye, Vec3D lookAt, Vec3D up) : eye(eye), lookAt(lookAt), up(up)
{
    Setup();
}

void Camera::setEyePoint(Point3D newEye)
{
    eye = newEye;
    Setup();
}
void Camera::setLookAt(Vec3D newLookAt)
{
    lookAt = newLookAt;
    Setup();
}
