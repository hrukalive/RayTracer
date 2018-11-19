/*
  ==============================================================================

    Disk.cpp
    Created: 17 Nov 2018 9:42:51pm
    Author:  bowen

  ==============================================================================
*/

#include "Disk.h"
#include "../Globals.h"

void Disk::UpdateBoundingBox()
{
    FP_TYPE deltax = r * sqrt(1 - pow(n * X_DIR, 2));
    FP_TYPE deltay = r * sqrt(1 - pow(n * Y_DIR, 2));
    FP_TYPE deltaz = r * sqrt(1 - pow(n * Z_DIR, 2));

    Point3D p1 = Point3D(center.x - deltax, center.y - deltay, center.z - deltaz);
    Point3D p2 = Point3D(center.x + deltax, center.y + deltay, center.z + deltaz);
    boundingBox = BBox(p1, p2);
}

Disk::Disk(Point3D center, FP_TYPE radius, Vec3D normal) : center(center), r(radius), n(normal.normalised())
{
    UpdateBoundingBox();
}

HitRecord Disk::Hit(const Ray& ray)
{
    HitRecord record;
    FP_TYPE t = ((center - ray.Origin) * n) / (ray.Direction * n);
    if (t > kEpsilon)
    {
        auto p = ray.GetPoint(t);
        if ((center - p).length() < r)
        {
            record.Hit = true;
            record.Normal = (n * -ray.Direction > 0.0 ? n.normalised() : -n.normalised());
            record.NormalFlipped = n * ray.Direction > 0.0;
            record.HitPoint = p;
            record.MaterialPtr = materialPtr;
            record.Ray = ray;
            record.T = t;
        }
    }
    return record;
}

//std::vector<std::pair<Point3D, Vec3D>> Disk::Sample()
//{
//    auto samplePoints = sampler->SampleCircle(viewPlane->NumAreaLightSamples);
//    std::vector<std::pair<Point3D, Vec3D>> ret;
//    for (auto& shift : samplePoints)
//        ret.push_back(std::make_pair(Point3D(center + u * shift.getX() + v * shift.getY()), n.normalised()));
//    return ret;
//}
//
//std::pair<Point3D, Vec3D> Disk::SampleSingle()
//{
//    auto samplePoint = sampler->SampleSquareSingle();
//    return std::make_pair(Point3D(a + u * samplePoint.getX() + v * samplePoint.getY()), n.normalised());
//}
//
//FP_TYPE Disk::pdf(const HitRecord& record)
//{
//    return 1.0 / (PI * r * r);
//}
