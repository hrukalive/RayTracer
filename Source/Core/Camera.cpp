/*
  ==============================================================================

    Camera.cpp
    Created: 28 Sep 2018 10:59:05am
    Author:  Bowen Sun

  ==============================================================================
*/

#include "Camera.h"
#include "Globals.h"

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

OrthographicCamera::OrthographicCamera(Point3D eye, Vec3D lookAt, Vec3D up) : Camera(eye, lookAt, up) {}
std::vector<Ray> OrthographicCamera::CreateRay(int c, int r, FP_TYPE offset) const
{
    std::vector<Ray> ret;
    auto centerPos = viewPlane->GetPixelCenter(c, r);
    auto shifts = sampler->SampleSquare(viewPlane->NumPixelSamples);
    for (auto& shift : shifts)
    {
        auto tmp = centerPos + shift * viewPlane->PixelSize;
        ret.push_back(Ray(u * (tmp.x + offset) + v * tmp.y + eye, -w));
    }
    return ret;
}
Ray OrthographicCamera::CreateARay(int c, int r, FP_TYPE offset) const
{
    auto centerPos = viewPlane->GetPixelCenter(c, r);
    auto shift = sampler->SampleSquareSingle();
    auto tmp = centerPos + shift * viewPlane->PixelSize;
    return Ray(u * (tmp.x + offset) + v * tmp.y + eye, -w);
}

PinholeCamera::PinholeCamera(Point3D eye, Vec3D lookAt, Vec3D up, float dist) : Camera(eye, lookAt, up), dist(dist) {}
std::vector<Ray> PinholeCamera::CreateRay(int c, int r, FP_TYPE offset) const
{
    std::vector<Ray> ret;
    auto centerPos = viewPlane->GetPixelCenter(c, r);
    auto shifts = sampler->SampleSquare(viewPlane->NumPixelSamples);
    for (auto& shift : shifts)
    {
        auto tmp = centerPos + (shift * viewPlane->PixelSize);
        ret.push_back(Ray(eye, (u * (tmp.x + offset) + v * tmp.y - w * dist).normalised()));
    }
    return ret;
}
Ray PinholeCamera::CreateARay(int c, int r, FP_TYPE offset) const
{
    auto centerPos = viewPlane->GetPixelCenter(c, r);
    auto shift = sampler->SampleSquareSingle();
    auto tmp = centerPos + (shift * viewPlane->PixelSize);
    return Ray(eye, (u * (tmp.x + offset) + v * tmp.y - w * dist).normalised());
}

ThinLensCamera::ThinLensCamera(Point3D eye, Vec3D lookAt, Vec3D up, float dist, float focalDist, float lensRadius)
    : Camera(eye, lookAt, up), dist(dist), f(focalDist), radius(lensRadius) {}

std::vector<Ray> ThinLensCamera::CreateRay(int c, int r, FP_TYPE offset) const
{
    std::vector<Ray> ret;
    auto centerPos = viewPlane->GetPixelCenter(c, r);
    auto shifts = sampler->SampleSquare(viewPlane->NumPixelSamples);
    for (auto& shift : shifts)
    {
        auto tmp = centerPos + (shift * viewPlane->PixelSize);
        tmp.x = tmp.x * f / dist;
        tmp.y = tmp.y * f / dist;
        
        auto lensShifts = sampler->SampleCircle(viewPlane->NumLensSamples);
        for (auto& lensShift : lensShifts)
        {
            ret.push_back(Ray(eye + u * (lensShift.x * radius) + v * (lensShift.y * radius), 
                (u * (tmp.x - lensShift.x * radius + offset) + v * (tmp.y - lensShift.y * radius) - w * f).normalised()));
        }
    }
    return ret;
}

Ray ThinLensCamera::CreateARay(int c, int r, FP_TYPE offset) const
{
    auto centerPos = viewPlane->GetPixelCenter(c, r);
    auto shift = sampler->SampleSquareSingle();
    auto tmp = centerPos + (shift * viewPlane->PixelSize);
    tmp.x = tmp.x * f / dist;
    tmp.y = tmp.y * f / dist;

    auto lensShift = sampler->SampleCircleSingle();
    return Ray(eye + u * (lensShift.x * radius) + v * (lensShift.y * radius), 
        (u * (tmp.x - lensShift.x * radius + offset) + v * (tmp.y - lensShift.y * radius) - w * f).normalised());
}


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
