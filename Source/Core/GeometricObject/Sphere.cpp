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
            record.NormalFlipped = n * ray.Direction > 0.0;
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
            record.NormalFlipped = n * ray.Direction > 0.0;
            record.HitPoint = p;
            record.MaterialPtr = materialPtr;
            record.Ray = ray;
            record.T = t2;
        }
    }
    return record;
}

std::vector<HitRecord> Sphere::getHitInterval(const Ray& ray)
{
    auto temp = ray.Origin - Center;
    auto a = ray.Direction * ray.Direction;
    auto b = 2.0 * (temp * ray.Direction);
    auto c = temp * temp - Radius * Radius;
    auto disc = b * b - 4.0 * a * c;
    std::vector<HitRecord> ret;

    if (disc >= 0.0)
    {
        auto t1 = (-b - sqrt(disc)) / a * 0.5;
        auto t2 = (-b + sqrt(disc)) / a * 0.5;

        HitRecord record1;
        auto p = ray.GetPoint(t1);
        auto n = p - Center;
        record1.Hit = true;
        record1.Normal = (n * -ray.Direction > 0.0 ? n.normalised() : -n.normalised());
        record1.NormalFlipped = n * ray.Direction > 0.0;
        record1.HitPoint = p;
        record1.MaterialPtr = materialPtr;
        record1.Ray = ray;
        record1.T = t1;
        ret.push_back(record1);

        HitRecord record2;
        p = ray.GetPoint(t2);
        n = p - Center;
        record2.Hit = true;
        record2.Normal = (n * -ray.Direction > 0.0 ? n.normalised() : -n.normalised());
        record2.NormalFlipped = n * ray.Direction > 0.0;
        record2.HitPoint = p;
        record2.MaterialPtr = materialPtr;
        record2.Ray = ray;
        record2.T = t2;
        ret.push_back(record2);
    }
    return ret;
}
