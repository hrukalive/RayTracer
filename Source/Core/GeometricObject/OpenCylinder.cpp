/*
  ==============================================================================

    OpenCylinder.cpp
    Created: 17 Nov 2018 10:01:55pm
    Author:  bowen

  ==============================================================================
*/

#include "OpenCylinder.h"
void OpenCylinder::UpdateBoundingBox()
{
    Point3D deltar(r, r, r);
    Point3D deltay(0, h, 0);
    boundingBox = BBox(bc - deltar, bc + deltar + deltay);
}

OpenCylinder::OpenCylinder(Point3D bottomCenter, FP_TYPE radius, FP_TYPE height) : bc(bottomCenter), r(radius), h(height) { UpdateBoundingBox(); }

HitRecord OpenCylinder::Hit(const Ray& ray)
{
    HitRecord record;
    auto temp = ray.Origin - bc;
    auto a = ray.Direction.x * ray.Direction.x + ray.Direction.z * ray.Direction.z;
    auto b = 2.0 * (temp.x * ray.Direction.x + temp.z * ray.Direction.z);
    auto c = temp.x * temp.x + temp.z * temp.z - r * r;
    auto disc = b * b - 4.0 * a * c;

    if (disc >= 0.0)
    {
        auto t1 = (-b - sqrt(disc)) / a * 0.5;
        auto t2 = (-b + sqrt(disc)) / a * 0.5;

        auto p1 = ray.GetPoint(t1);
        auto p2 = ray.GetPoint(t2);
        if (t1 > kEpsilon && p1.y >= bc.y && p1.y < bc.y + h)
        {
            auto sphereN = p1 - bc;
            auto n = Vec3D(sphereN.x, 0, sphereN.z);
            record.Hit = true;
            record.Normal = (n * -ray.Direction > 0.0 ? n.normalised() : -n.normalised());
            record.NormalFlipped = n * ray.Direction > 0.0;
            record.HitPoint = p1;
            record.MaterialPtr = materialPtr;
            record.Ray = ray;
            record.T = t1;
        }
        else if (t2 > kEpsilon && p2.y >= bc.y && p2.y < bc.y + h)
        {
            auto sphereN = p2 - bc;
            auto n = Vec3D(sphereN.x, 0, sphereN.z);
            record.Hit = true;
            record.Normal = (n * -ray.Direction > 0.0 ? n.normalised() : -n.normalised());
            record.NormalFlipped = n * ray.Direction > 0.0;
            record.HitPoint = p2;
            record.MaterialPtr = materialPtr;
            record.Ray = ray;
            record.T = t2;
        }
    }
    return record;
}
