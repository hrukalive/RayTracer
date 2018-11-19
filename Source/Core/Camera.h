/*
  ==============================================================================

    Camera.h
    Created: 28 Sep 2018 10:59:05am
    Author:  Bowen Sun

  ==============================================================================
*/

#pragma once
#include "../../JuceLibraryCode/JuceHeader.h"
#include "Utility.h"

class Camera
{
protected:
    Point3D eye;
    Vec3D lookAt;
    Vec3D up;
    
    Vec3D w, u, v;

    void Setup();
public:
    Camera(Point3D eye, Vec3D lookAt, Vec3D up);
    virtual ~Camera() = default;

    void setEyePoint(Point3D newEye);
    void setLookAt(Vec3D newLookAt);

    virtual std::vector<Ray> CreateRay(int c, int r, FP_TYPE offset = 0) const = 0;
    virtual Ray CreateARay(int c, int r, FP_TYPE offset = 0) const = 0;
};

class OrthographicCamera : public Camera
{
public:
    OrthographicCamera(Point3D eye, Vec3D lookAt, Vec3D up);
    virtual ~OrthographicCamera() = default;

    virtual std::vector<Ray> CreateRay(int c, int r, FP_TYPE offset = 0) const;
    virtual Ray CreateARay(int c, int r, FP_TYPE offset = 0) const;
};

class PinholeCamera : public Camera
{
    float dist;
public:
    PinholeCamera(Point3D eye, Vec3D lookAt, Vec3D up, float dist);
    virtual ~PinholeCamera() = default;

    virtual std::vector<Ray> CreateRay(int c, int r, FP_TYPE offset = 0) const;
    virtual Ray CreateARay(int c, int r, FP_TYPE offset = 0) const;
};

class ThinLensCamera : public Camera
{
    float dist;
    float f;
    float radius;
public:
    ThinLensCamera(Point3D eye, Vec3D lookAt, Vec3D up, float dist, float focalDist, float lensRadius);
    virtual ~ThinLensCamera() = default;
    
    virtual std::vector<Ray> CreateRay(int c, int r, FP_TYPE offset = 0) const;
    virtual Ray CreateARay(int c, int r, FP_TYPE offset = 0) const;
};

class StereoCamera : public Camera
{
    std::shared_ptr<Camera> leftCamera, rightCamera;
    FP_TYPE beta, diff;
public:
    StereoCamera(Point3D eye, Vec3D lookAt, Vec3D up, std::shared_ptr<Camera> leftCamera, std::shared_ptr<Camera> rightCamera, FP_TYPE beta, bool sameLookAt = false);
    virtual ~StereoCamera() = default;

    virtual std::vector<Ray> CreateRay(int c, int r, FP_TYPE offset = 0) const;
    virtual Ray CreateARay(int c, int r, FP_TYPE offset = 0) const;
};
