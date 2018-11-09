#include "Rectangle.h"
#include "../Globals.h"

namespace RayTracer
{
    void Rectangle::UpdateBoundingBox()
    {
        boundingBox = BBox(a, a);
        boundingBox.Merge(BBox(a + u, a + u));
        boundingBox.Merge(BBox(a + v, a + v));
        boundingBox.Merge(BBox(a + u + v, a + u + v));
    }

    Rectangle::Rectangle(Point3D origin, Vec3D uDirection, Vec3D vDirection)
        : a(origin), u(uDirection), v(vDirection)
    {
        n = u ^ v;
        UpdateBoundingBox();
    }

    void Rectangle::setOrigin(Point3D origin) { a = origin; UpdateBoundingBox(); }
    void Rectangle::setU(Vec3D uDirection) { u = uDirection; n = u ^ v; UpdateBoundingBox(); }
    void Rectangle::setV(Vec3D vDirection) { v = vDirection; n = u ^ v; UpdateBoundingBox(); }
    Point3D Rectangle::getOrigin() { return a; }
    Vec3D Rectangle::getU() { return u; }
    Vec3D Rectangle::getV() { return v; }
    Vec3D Rectangle::GetNormal(const Point3D p) { return n.normalised(); }

    std::vector<std::pair<Point3D, Vec3D>> Rectangle::Sample()
    {
        auto samplePoints = sampler->SampleSquare(viewPlane->NumAreaLightSamples);
        std::vector<std::pair<Point3D, Vec3D>> ret;
        for (auto& shift : samplePoints)
            ret.push_back(std::make_pair(Point3D(a + u * shift.getX() + v * shift.getY()), n.normalised()));
        return ret;
    }

    std::pair<Point3D, Vec3D> Rectangle::SampleSingle()
    {
        auto samplePoint = sampler->SampleSquareSingle();
        return std::make_pair(Point3D(a + u * samplePoint.getX() + v * samplePoint.getY()), n.normalised());
    }

    FP_TYPE Rectangle::pdf(const HitRecord& record)
    {
        return 1.0 / (u * v);
    }

    HitRecord Rectangle::Hit(const Ray& ray)
    {
        HitRecord record;
        FP_TYPE t = ((a - ray.Origin) * n) / (ray.Direction * n);
        auto p = ray.GetPoint(t);
        auto tu = (p - a) * u.normalised() / u.length();
        auto tv = (p - a) * v.normalised() / v.length();

        if (t > kEpsilon && tu >= 0.0 && tu <= 1.0 && tv >= 0.0 && tv <= 1.0)
        {
            record.Hit = true;
            record.Normal = (n * -ray.Direction > 0.0 ? n.normalised() : -n.normalised());
            record.NormalFlipped = n * ray.Direction > 0.0;
            record.HitPoint = p;
            record.MaterialPtr = materialPtr;
            record.Ray = ray;
            record.T = t;
        }
        return record;
    }
}
