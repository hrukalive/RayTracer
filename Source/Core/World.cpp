/*
  ==============================================================================

    World.cpp
    Created: 28 Sep 2018 11:07:29am
    Author:  Bowen Sun

  ==============================================================================
*/

#include "World.h"
#include <climits>

std::shared_ptr<Light>& World::GetAmbientLightPtr() { return ambientLightPtr; }
RGBColor& World::GetBackgroundColor() { return backgroundColor; }
std::vector<std::shared_ptr<GeometricObject>>& World::GetObjects() { return objects; }
std::vector<std::shared_ptr<Light>>& World::GetLights() { return lights; }

void World::SetAmbient(std::shared_ptr<Light>& ambient) { ambientLightPtr = ambient; }
void World::AddLight(std::shared_ptr<Light>& light) { lights.push_back(light); }
void World::AddObject(std::shared_ptr<GeometricObject>& obj)
{ 
    objects.push_back(obj);

    if (objects.size() == 1)
        boundingBox = objects[0]->GetBoundingBox();
    else if (objects.size() > 1)
        boundingBox.Merge(objects[objects.size() - 1]->GetBoundingBox());
}

FP_TYPE World::TotalLightPower()
{
    FP_TYPE acc = 0;
    for (auto& l : lights)
        acc += l->GetPower();
    return acc;
}

BBox World::GetBoundingBox()
{
    return boundingBox;
}

void World::Clear()
{
    ambientLightPtr = nullptr;
    objects.clear();
    lights.clear();
    boundingBox = BBox();
}

HitRecord World::HitObjects(const Ray& ray)
{
    HitRecord record;
    FP_TYPE tmin = INFINITY;

    for (int i = 0; i < objects.size(); i++)
    {
        HitRecord tmp = objects[i]->Hit(ray);
        if (tmp.Hit && tmp.T < tmin)
        {
            tmin = tmp.T;
            record = tmp;
        }
    }
    return record;
}
