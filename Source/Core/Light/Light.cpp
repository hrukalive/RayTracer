/*
  ==============================================================================

    Light.cpp
    Created: 6 Dec 2018 3:05:00pm
    Author:  bowen

  ==============================================================================
*/

#include "Light.h"

std::shared_ptr<Light> Light::parse(StringArray& cmd, std::unordered_map<String, std::shared_ptr<void>>& env)
{
    if (cmd[1] == "Ambient")
        return Ambient::parse(cmd, env);
    else if (cmd[1] == "PointLight")
        return PointLight::parse(cmd, env);
    else if (cmd[1] == "ParallelLight")
        return ParallelLight::parse(cmd, env);
    else if (cmd[1] == "AreaLight")
        return AreaLight::parse(cmd, env);
    else
        throw std::invalid_argument("[LIGHT] Type not found.");
}

std::shared_ptr<Light> Ambient::parse(StringArray& cmd, std::unordered_map<String, std::shared_ptr<void>>& env)
{
    return std::make_shared<Ambient>(Ambient(
        std::stod(cmd[3].toStdString()),
        RGBColor(
            std::stod(cmd[4].toStdString()),
            std::stod(cmd[5].toStdString()),
            std::stod(cmd[6].toStdString())
        )
    ));
}

std::shared_ptr<Light> ParallelLight::parse(StringArray& cmd, std::unordered_map<String, std::shared_ptr<void>>& env)
{
    return std::make_shared<ParallelLight>(ParallelLight(
        std::stod(cmd[3].toStdString()),
        RGBColor(
            std::stod(cmd[4].toStdString()),
            std::stod(cmd[5].toStdString()),
            std::stod(cmd[6].toStdString())
        ),
        Vec3D(
            std::stod(cmd[7].toStdString()),
            std::stod(cmd[8].toStdString()),
            std::stod(cmd[9].toStdString())
        )
    ));
}

std::shared_ptr<Light> PointLight::parse(StringArray& cmd, std::unordered_map<String, std::shared_ptr<void>>& env)
{
    return std::make_shared<PointLight>(PointLight(
        std::stod(cmd[3].toStdString()),
        RGBColor(
            std::stod(cmd[4].toStdString()),
            std::stod(cmd[5].toStdString()),
            std::stod(cmd[6].toStdString())
        ),
        Point3D(
            std::stod(cmd[7].toStdString()),
            std::stod(cmd[8].toStdString()),
            std::stod(cmd[9].toStdString())
        ),
        std::stod(cmd[10].toStdString())
    ));
}

std::shared_ptr<Light> AreaLight::parse(StringArray& cmd, std::unordered_map<String, std::shared_ptr<void>>& env)
{
    if (env.find(cmd[3]) == env.end() || env.find(cmd[4]) == env.end())
        throw std::invalid_argument("[AREA LIGHT] Required pointer not found.");
    return std::make_shared<AreaLight>(AreaLight(
        std::static_pointer_cast<GeometricObject>(env[cmd[3]]),
        std::static_pointer_cast<Material>(env[cmd[4]])
    ));
}
