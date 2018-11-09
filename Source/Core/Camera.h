/*
  ==============================================================================

    Camera.h
    Created: 28 Sep 2018 10:59:05am
    Author:  Bowen Sun

  ==============================================================================
*/

#pragma once
#include "../../JuceLibraryCode/JuceHeader.h"
#include "Viewplane.h"
#include "Sampler.h"

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
    Camera(Point3D eye, Vec3D lookAt, Vec3D up, std::shared_ptr<ViewPlane>& viewPlane, std::shared_ptr<Sampler>& sampler);
    virtual ~Camera() = default;

    virtual std::vector<Ray> CreateRay(int c, int r) const = 0;
    virtual Ray CreateARay(int c, int r) const = 0;
};

class OrthographicCamera : public Camera
{
public:
    OrthographicCamera(Point3D eye, Vec3D lookAt, Vec3D up, std::shared_ptr<ViewPlane>& viewPlane, std::shared_ptr<Sampler>& sampler);
    virtual ~OrthographicCamera() = default;

    virtual std::vector<Ray> CreateRay(int c, int r) const;
    virtual Ray CreateARay(int c, int r) const;
};

class PinholeCamera : public Camera
{
    float dist;
public:
    PinholeCamera(Point3D eye, Vec3D lookAt, Vec3D up, float dist, std::shared_ptr<ViewPlane>& viewPlane, std::shared_ptr<Sampler>& sampler);
    virtual ~PinholeCamera() = default;

    virtual std::vector<Ray> CreateRay(int c, int r) const;
    virtual Ray CreateARay(int c, int r) const;
};

class ThinLensCamera : public Camera
{
    float dist;
    float f;
    float radius;
public:
    ThinLensCamera(Point3D eye, Vec3D lookAt, Vec3D up, float dist, float focalDist, float lensRadius,
                   std::shared_ptr<ViewPlane>& viewPlane, std::shared_ptr<Sampler>& sampler);
    virtual ~ThinLensCamera() = default;
    
    virtual std::vector<Ray> CreateRay(int c, int r) const;
    virtual Ray CreateARay(int c, int r) const;
};
