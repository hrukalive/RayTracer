/*
  ==============================================================================

    Light.cpp
    Created: 28 Sep 2018 11:03:21am
    Author:  Bowen Sun

  ==============================================================================
*/

#include "Globals.h"
#include "Light.h"
#include "World.h"
#include "Materials/Material.h"
#include "GeometricObject/GeometricObject.h"

std::vector<Ray> Light::EmitPhoton()
{
    return std::vector<Ray>();
}

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

AreaLight::AreaLight(std::shared_ptr<GeometricObject> obj, std::shared_ptr<Material> material)
    : ObjPtr(obj), MaterialPtr(material) {}

bool AreaLight::InShadow(const Ray& ray, const Point3D samplePoint, const HitRecord& record)
{
    auto& objs = world->GetObjects();
    auto d = (samplePoint - ray.Origin) * ray.Direction.normalised();
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

bool AreaLight::InShadow(const Ray& ray, const HitRecord& record)
{
    return InShadow(ray, samplePoint, record);
}

std::vector<std::pair<Point3D, std::pair<Vec3D, RGBColor>>> AreaLight::GetWiAndLGPDF(const HitRecord& record)
{
    std::vector<std::pair<Point3D, std::pair<Vec3D, RGBColor>>> ret;
    auto samples = ObjPtr->Sample();
    for (auto& sample : samples)
    {
        auto samplePoint = sample.first;
        auto lightNormal = sample.second;
        auto wi = (samplePoint - record.HitPoint).normalised();

        FP_TYPE ndotd = -lightNormal * wi;
        if (ndotd > 0.0)
        {
            auto L = MaterialPtr->GetLe(record);
            FP_TYPE d2 = decay * (samplePoint - record.HitPoint).lengthSquared() + 1;
            auto G = ndotd / d2;
            FP_TYPE pdf = ObjPtr->pdf(record);
            ret.push_back(std::make_pair(samplePoint, std::make_pair(wi, L * G * pdf)));
        }
        else
        {
            ret.push_back(std::make_pair(samplePoint, std::make_pair(wi, BLACK)));
        }
    }
    return ret;
}

std::vector<Ray> AreaLight::EmitPhoton()
{
    std::vector<Ray> ret;
    int ne = GetPower() * TOTAL_PHOTON / world->TotalLightPower();
    auto backupSampler = sampler;
    sampler.reset(new Hammersley());
    auto samples = ObjPtr->Sample(ne);
    auto dirs = sampler->SampleHemisphere(ne, 1);
    Random rand;
    for (size_t i = 0; i < samples.size(); i++)
    {
        auto& sample = samples[i];
        auto samplePoint = sample.first;
        auto r = sample.second;

        auto w = r.normalised();
        auto u = (Vec3D(0, 1, 0) ^ w).normalised();
        auto v = (w ^ u).normalised();
        if (abs(r.x) < 1e-7 && abs(r.z) < 1e-7)
        {
            if (r.y > 0)
            {
                u = Vec3D(0.0, 0.0, 1.0);
                v = Vec3D(1.0, 0.0, 0.0);
                w = Vec3D(0.0, 1.0, 0.0);
            }
            else
            {
                u = Vec3D(1.0, 0.0, 0.0);
                v = Vec3D(0.0, 0.0, 1.0);
                w = Vec3D(0.0, -1.0, 0.0);
            }
        }
        auto& hemisample = dirs[i];
        wi = u * hemisample.x + v * hemisample.y + w * hemisample.z;
        ret.push_back(Ray(samplePoint, (u * hemisample.x + v * hemisample.y + w * hemisample.z).normalised(), MaterialPtr->GetLe(HitRecord())));
    }
    sampler = backupSampler;
    return ret;
}

void AreaLight::setLs(FP_TYPE ls)
{
    std::dynamic_pointer_cast<Emissive>(MaterialPtr)->SetLs(ls);
}

FP_TYPE AreaLight::GetPower()
{
    return MaterialPtr->GetLe(HitRecord()).lengthSquared();
}

Vec3D AreaLight::GetDirection(const HitRecord& record)
{
    auto sample = ObjPtr->SampleSingle();
    samplePoint = sample.first;
    lightNormal = sample.second;
    wi = (samplePoint - record.HitPoint).normalised();
    return wi;
}

RGBColor AreaLight::L(const HitRecord& record)
{
    FP_TYPE ndotd = -lightNormal * wi;
    if (ndotd > 0.0)
        return MaterialPtr->GetLe(record);
    else
        return BLACK;
}

FP_TYPE AreaLight::G(const HitRecord& record) const
{
    FP_TYPE ndotd = -lightNormal * wi;
    FP_TYPE d2 = decay * (samplePoint - record.HitPoint).lengthSquared() + 1;
    return ndotd / d2;
}

FP_TYPE AreaLight::pdf(const HitRecord& record) const
{
    return ObjPtr->pdf(record);
}
