/*
  ==============================================================================

    GeometricObject.cpp
    Created: 18 Nov 2018 4:20:09pm
    Author:  bowen

  ==============================================================================
*/

#include "GeometricObject.h"
#include "../Globals.h"

void GeometricObject::SetMaterial(std::shared_ptr<Material> newMaterialPtr) {
    materialPtr = newMaterialPtr;
}

BBox& GeometricObject::GetBoundingBox() {
    return boundingBox;
}

std::vector<std::pair<Point3D, Vec3D>> GeometricObject::Sample(int numSamples)
{
    return std::vector<std::pair<Point3D, Vec3D>>();
}

std::pair<Point3D, Vec3D> GeometricObject::SampleSingle()
{
    return std::make_pair(ZERO, ZERO);
}

FP_TYPE GeometricObject::pdf(const HitRecord& record)
{
    auto diff = boundingBox.GetMaxPoint() - boundingBox.GetMinPoint();
    return 1.0 / (2 * (diff.x * diff.y + diff.y * diff.z + diff.x * diff.z));
}

std::vector<HitRecord> GeometricObject::getHitInterval(const Ray& ray)
{
    BBox box = world->GetBoundingBox();
    FP_TYPE dist = std::max((ray.Origin - box.GetMinPoint()).length(), (ray.Origin - box.GetMaxPoint()).length());
    dist += (box.GetMaxPoint() - box.GetMinPoint()).length();

    std::vector<HitRecord> ret;
    FP_TYPE tmpT = -dist;
    Ray tmpRay = ray;
    FP_TYPE acc = -dist;
    while (true)
    {
        tmpRay = Ray(tmpRay.GetPoint(tmpT + kEpsilon), ray.Direction);
        HitRecord record = Hit(tmpRay);
        if (!record.Hit)
            return ret;
        tmpT = record.T;
        record.T += acc;
        acc = record.T;
        ret.push_back(record);
        tmpRay = Ray(tmpRay.GetPoint(tmpT + kEpsilon), ray.Direction);
        record = Hit(tmpRay);
        if (!record.Hit)
        {
            ret.push_back(ret[ret.size() - 1]);
            return ret;
        }
        else
        {
            tmpT = record.T;
            record.T += acc;
            acc = record.T;
            ret.push_back(record);
        }
    }
    return ret;
}
