#include "Sphere.h"

void Sphere::UpdateBoundingBox()
{
    Point3D delta(Radius, Radius, Radius);
    boundingBox = BBox(Center - delta, Center + delta);
}

Sphere::Sphere(Point3D center, FP_TYPE radius) : Center(center), Radius(radius) { UpdateBoundingBox(); }

void Sphere::setCenter(Point3D center) { Center = center; UpdateBoundingBox(); }
void Sphere::setRadius(FP_TYPE radius) { Radius = radius; UpdateBoundingBox(); }
Point3D Sphere::getCenter() { return Center; }
FP_TYPE Sphere::getRadius() { return Radius; }

HitRecord Sphere::Hit(const Ray& ray)
{
    HitRecord record;
    auto temp = ray.Origin - Center;
    auto a = ray.Direction * ray.Direction;
    auto b = 2.0 * (temp * ray.Direction);
    auto c = temp * temp - Radius * Radius;
    auto disc = b * b - 4.0 * a * c;

    if (disc >= 0.0)
    {
        auto t1 = (-b - sqrt(disc)) / a * 0.5;
        auto t2 = (-b + sqrt(disc)) / a * 0.5;

        if (t1 > kEpsilon)
        {
            auto p = ray.GetPoint(t1);
            auto n = p - Center;
            record.Hit = true;
            record.Normal = (n * -ray.Direction > 0.0 ? n.normalised() : -n.normalised());
            record.HitPoint = p;
            record.MaterialPtr = materialPtr;
            record.Ray = ray;
            record.T = t1;
        }
        else if (t2 > kEpsilon)
        {
            auto p = ray.GetPoint(t2);
            auto n = p - Center;
            record.Hit = true;
            record.Normal = (n * -ray.Direction > 0.0 ? n.normalised() : -n.normalised());
            record.HitPoint = p;
            record.MaterialPtr = materialPtr;
            record.Ray = ray;
            record.T = t2;
        }
    }
    return record;
}
