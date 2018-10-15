/*
  ==============================================================================

    Camera.cpp
    Created: 28 Sep 2018 10:59:05am
    Author:  Bowen Sun

  ==============================================================================
*/

#include "Camera.h"

Camera::Camera(Point3D eye, Vec3D lookAt, Vec3D up, std::shared_ptr<ViewPlane>& viewPlane, std::shared_ptr<Sampler>& sampler)
    : eye(eye), lookAt(lookAt), up(up), viewPlane(viewPlane), sampler(sampler)
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

OrthographicCamera::OrthographicCamera(Point3D eye, Vec3D lookAt, Vec3D up, std::shared_ptr<ViewPlane>& viewPlane, std::shared_ptr<Sampler>& sampler)
    : Camera(eye, lookAt, up, viewPlane, sampler) {}
std::vector<Ray> OrthographicCamera::CreateRay(int c, int r) const
{
    std::vector<Ray> ret;
    auto centerPos = viewPlane->GetPixelCenter(c, r);
    auto shifts = sampler->SampleSquare(viewPlane->NumPixelSamples);
    for (auto& shift : shifts)
    {
        auto tmp = centerPos + shift * viewPlane->PixelSize;
        ret.push_back(Ray(u * tmp.x + v * tmp.y + eye, -w));
    }
    return ret;
}

PinholeCamera::PinholeCamera(Point3D eye, Vec3D lookAt, Vec3D up, float dist, std::shared_ptr<ViewPlane>& viewPlane, std::shared_ptr<Sampler>& sampler)
    : Camera(eye, lookAt, up, viewPlane, sampler), dist(dist) {}
std::vector<Ray> PinholeCamera::CreateRay(int c, int r) const
{
    std::vector<Ray> ret;
    auto centerPos = viewPlane->GetPixelCenter(c, r);
    auto shifts = sampler->SampleSquare(viewPlane->NumPixelSamples);
    for (auto& shift : shifts)
    {
        auto tmp = centerPos + (shift * viewPlane->PixelSize);
        ret.push_back(Ray(eye, (u * tmp.x + v * tmp.y - w * dist).normalised()));
    }
    return ret;
}

ThinLensCamera::ThinLensCamera(Point3D eye, Vec3D lookAt, Vec3D up, float dist, float focalDist, float lensRadius,
                               std::shared_ptr<ViewPlane>& viewPlane, std::shared_ptr<Sampler>& sampler)
    : Camera(eye, lookAt, up, viewPlane, sampler), dist(dist), f(focalDist), radius(lensRadius) {}

std::vector<Ray> ThinLensCamera::CreateRay(int c, int r) const
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
            ret.push_back(Ray(eye, (u * (tmp.x - lensShift.x * radius) + v * (tmp.y - lensShift.y * radius) - w * f).normalised()));
        }
    }
    return ret;
}
