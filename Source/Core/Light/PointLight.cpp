#include "../Globals.h"
#include "../World.h"
#include "../Globals.h"
#include "Light.h"

bool PointLight::InShadow(const Ray& ray, const HitRecord& record)
{
    auto& objs = world->GetObjects();
    auto d = (location - ray.Origin).length();
    for (int i = 0; i < objs.size(); i++)
    {
        auto tmp = objs[i]->Hit(ray);
        if (tmp.Hit && tmp.T < d)
        {
            return true;
        }
    }
    return false;
}

std::vector<Ray> PointLight::EmitPhoton()
{
    std::vector<Ray> ret;
    Random rand;
    int ne = round(GetPower() * TOTAL_PHOTON / world->TotalLightPower());
    for (int i = 0; i < ne; i++)
    {
        FP_TYPE x, y, z;
        do
        {
            x = 1 - 2 * rand.nextDouble();
            y = 1 - 2 * rand.nextDouble();
            z = 1 - 2 * rand.nextDouble();
        } while (x * x + y * y + z * z > 1);
        ret.push_back(Ray(location, Vec3D(x, y, z).normalised(), color * ls));
    }
    return ret;
}
