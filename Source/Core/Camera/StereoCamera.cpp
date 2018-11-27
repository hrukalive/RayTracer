#include "Camera.h"
#include "../Globals.h"

StereoCamera::StereoCamera(Point3D eye, Vec3D lookAt, Vec3D up, 
    std::shared_ptr<Camera> leftCamera, std::shared_ptr<Camera> rightCamera, FP_TYPE beta, bool sameLookAt)
    : Camera(eye, lookAt, up), leftCamera(leftCamera), rightCamera(rightCamera), beta(beta), sameLookAt(sameLookAt)
{
    auto rad = 0.5 * beta * PI_OVER_180;
    diff = (eye - lookAt).length() * tan(rad);
    if (sameLookAt)
    {
        leftCamera->setLookAt(lookAt);
        leftCamera->setEyePoint(eye - u * (diff * cos(rad) * cos(rad)) - w * (diff * cos(rad) * sin(rad)));
        rightCamera->setLookAt(lookAt);
        rightCamera->setEyePoint(eye + u * (diff * cos(rad) * cos(rad)) - w * (diff * cos(rad) * sin(rad)));
    }
    else
    {
        leftCamera->setEyePoint(eye - u * diff);
        leftCamera->setLookAt(lookAt - u * diff);
        rightCamera->setEyePoint(eye + u * diff);
        rightCamera->setLookAt(lookAt + u * diff);
    }
    if (sameLookAt)
        diff = 0;
}

std::vector<Ray> StereoCamera::CreateRay(int c, int r, FP_TYPE offset) const
{
    return (c < viewPlane->Width ? rightCamera->CreateRay(c, r, -diff) : leftCamera->CreateRay(c - viewPlane->Width, r, diff));
}

Ray StereoCamera::CreateARay(int c, int r, FP_TYPE offset) const
{
    return (c < viewPlane->Width ? rightCamera->CreateARay(c, r, -diff) : leftCamera->CreateARay(c - viewPlane->Width, r, diff));
}
