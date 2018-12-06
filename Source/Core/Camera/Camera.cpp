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

std::shared_ptr<Camera> Camera::parse(StringArray& cmd, std::unordered_map<String, std::shared_ptr<void>>& env)
{
    if (cmd[2] == "OrthographicCamera")
        return OrthographicCamera::parse(cmd, env);
    else if (cmd[2] == "PinholeCamera")
        return PinholeCamera::parse(cmd, env);
    else if (cmd[2] == "ThinLensCamera")
        return ThinLensCamera::parse(cmd, env);
    else if (cmd[2] == "StereoCamera")
        return StereoCamera::parse(cmd, env);
    else
        throw std::invalid_argument("Invalid camera type.");
}

std::shared_ptr<Camera> OrthographicCamera::parse(StringArray& cmd, std::unordered_map<String, std::shared_ptr<void>>& env)
{
    auto r = std::stod(cmd[7].toStdString());
    auto theta = std::stod(cmd[8].toStdString()) * PI_OVER_180;
    auto phi = std::stod(cmd[9].toStdString()) * PI_OVER_180;
    auto roll = std::stod(cmd[10].toStdString()) * PI_OVER_180;
    auto lookat = Vec3D(std::stod(cmd[4].toStdString()), std::stod(cmd[5].toStdString()), std::stod(cmd[6].toStdString()));
    auto eyepoint = Vec3D(r * sin(theta) * sin(phi), r * cos(phi), r * cos(theta) * sin(phi)) + lookat;

    return std::make_shared<OrthographicCamera>(OrthographicCamera(eyepoint, lookat, Vec3D(sin(roll), cos(roll), 0.0)));
}

std::shared_ptr<Camera> PinholeCamera::parse(StringArray& cmd, std::unordered_map<String, std::shared_ptr<void>>& env)
{
    auto r = std::stod(cmd[7].toStdString());
    auto theta = std::stod(cmd[8].toStdString()) * PI_OVER_180;
    auto phi = std::stod(cmd[9].toStdString()) * PI_OVER_180;
    auto roll = std::stod(cmd[10].toStdString()) * PI_OVER_180;
    auto lookat = Vec3D(std::stod(cmd[4].toStdString()), std::stod(cmd[5].toStdString()), std::stod(cmd[6].toStdString()));
    auto eyepoint = Vec3D(r * sin(theta) * sin(phi), r * cos(phi), r * cos(theta) * sin(phi)) + lookat;

    return std::make_shared<PinholeCamera>(PinholeCamera(eyepoint, lookat, Vec3D(sin(roll), cos(roll), 0.0), std::stod(cmd[11].toStdString())));
}

std::shared_ptr<Camera> StereoCamera::parse(StringArray& cmd, std::unordered_map<String, std::shared_ptr<void>>& env)
{
    auto r = std::stod(cmd[7].toStdString());
    auto theta = std::stod(cmd[8].toStdString()) * PI_OVER_180;
    auto phi = std::stod(cmd[9].toStdString()) * PI_OVER_180;
    auto roll = std::stod(cmd[10].toStdString()) * PI_OVER_180;
    auto lookat = Vec3D(std::stod(cmd[4].toStdString()), std::stod(cmd[5].toStdString()), std::stod(cmd[6].toStdString()));
    auto eyepoint = Point3D(r * sin(theta) * sin(phi), r * cos(phi), r * cos(theta) * sin(phi)) + lookat;

    auto lc = env.find(cmd[11]), rc = env.find(cmd[12]);
    if (lc == env.end() || rc == env.end())
        throw std::invalid_argument("Required camera pointer not found.");

    return std::make_shared<StereoCamera>(StereoCamera(eyepoint, lookat, Vec3D(sin(roll), cos(roll), 0.0),
        std::static_pointer_cast<Camera>(lc->second), std::static_pointer_cast<Camera>(rc->second),
        std::stod(cmd[13].toStdString()), cmd[14] == "true"));
}

std::shared_ptr<Camera> ThinLensCamera::parse(StringArray& cmd, std::unordered_map<String, std::shared_ptr<void>>& env)
{
    auto r = std::stod(cmd[7].toStdString());
    auto theta = std::stod(cmd[8].toStdString()) * PI_OVER_180;
    auto phi = std::stod(cmd[9].toStdString()) * PI_OVER_180;
    auto roll = std::stod(cmd[10].toStdString()) * PI_OVER_180;
    auto lookat = Vec3D(std::stod(cmd[4].toStdString()), std::stod(cmd[5].toStdString()), std::stod(cmd[6].toStdString()));
    auto eyepoint = Vec3D(r * sin(theta) * sin(phi), r * cos(phi), r * cos(theta) * sin(phi)) + lookat;

    return std::make_shared<ThinLensCamera>(ThinLensCamera(eyepoint, lookat, Vec3D(sin(roll), cos(roll), 0.0),
        std::stod(cmd[11].toStdString()), std::stod(cmd[12].toStdString()), std::stod(cmd[13].toStdString())));
}
