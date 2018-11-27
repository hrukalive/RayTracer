#include "../Globals.h"
#include "../World.h"
#include "../Globals.h"
#include "Light.h"

bool ParallelLight::InShadow(const Ray& ray, const HitRecord& record)
{
    auto& objs = world->GetObjects();
    for (int i = 0; i < objs.size(); i++)
    {
        if (objs[i]->Hit(ray).Hit)
        {
            return true;
        }
    }
    return false;
}
