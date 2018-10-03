/*
  ==============================================================================

    Camera.h
    Created: 28 Sep 2018 10:59:05am
    Author:  Bowen Sun

  ==============================================================================
*/

#pragma once
#include "Utility.h"
#include "Viewplane.h"
#include "Sampler.h"
#include <string>

class Camera
{
protected:
    Point3D eye;
    Vec3D lookAt;
    Vec3D up;
    std::shared_ptr<ViewPlane> viewPlane;
    std::shared_ptr<Sampler> sampler;
    
    Vec3D w, u, v;
public:
    Camera(Point3D eye, Vec3D lookAt, Vec3D up, std::shared_ptr<ViewPlane>& viewPlane, std::shared_ptr<Sampler>& sampler)
        : eye(eye), lookAt(lookAt), up(up), viewPlane(viewPlane), sampler(sampler)
    {
        w = (eye - lookAt).normalised();
        u = (up ^ w).normalised();
        v = w ^ u;
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
    virtual ~Camera() {}
    virtual std::vector<Ray> CreateRay(int c, int r) const = 0;
};

class OrthographicCamera : public Camera
{
public:
    OrthographicCamera(Point3D eye, Vec3D lookAt, Vec3D up, std::shared_ptr<ViewPlane>& viewPlane, std::shared_ptr<Sampler>& sampler)
        : Camera(eye, lookAt, up, viewPlane, sampler) {}
    virtual ~OrthographicCamera() {}
    virtual std::vector<Ray> CreateRay(int c, int r) const
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
};

class PinholeCamera : public Camera
{
    float dist;
public:
    PinholeCamera(Point3D eye, Vec3D lookAt, Vec3D up, float dist, std::shared_ptr<ViewPlane>& viewPlane, std::shared_ptr<Sampler>& sampler)
    : Camera(eye, lookAt, up, viewPlane, sampler), dist(dist) {}
    virtual ~PinholeCamera() {}
    virtual std::vector<Ray> CreateRay(int c, int r) const
    {
        std::vector<Ray> ret;
        auto centerPos = viewPlane->GetPixelCenter(c, r);
        auto shifts = sampler->SampleSquare(viewPlane->NumPixelSamples);
        for (auto& shift : shifts)
        {
            auto tmp = centerPos + shift * viewPlane->PixelSize;
			// DBG((std::to_string(tmp.x) + ", " + std::to_string(tmp.y)));
            ret.push_back(Ray(eye, (u * tmp.x + v * tmp.y + eye - w * dist).normalised()));
        }
        return ret;
    }
};
