#include "Triangle.h"

void Triangle::Triangle::UpdateNormal()
{
    n = (b - a) ^ (c - a);
}

void Triangle::UpdateBoundingBox()
{
    boundingBox = BBox(a, a);
    boundingBox.Merge(BBox(b, b));
    boundingBox.Merge(BBox(c, c));
}

Triangle::Triangle(Vec3D vertexA, Vec3D vertexB, Vec3D vertexC)
    : a(vertexA), b(vertexB), c(vertexC)
{
    UpdateNormal();
    UpdateBoundingBox();
}

void Triangle::setA(Vec3D vertexA) { a = vertexA; UpdateNormal(); UpdateBoundingBox(); }
void Triangle::setB(Vec3D vertexB) { b = vertexB; UpdateNormal(); UpdateBoundingBox(); }
void Triangle::setC(Vec3D vertexC) { c = vertexC; UpdateNormal(); UpdateBoundingBox(); }
Vec3D Triangle::getA() { return a; }
Vec3D Triangle::getB() { return b; }
Vec3D Triangle::getC() { return c; }
Vec3D Triangle::getNormal() { return n.normalised(); }

HitRecord Triangle::Hit(const Ray& ray)
{
    HitRecord record;
    auto temp = ((a - ray.Origin) * n) / (ray.Direction * n);
    if (temp > kEpsilon)
    {
        auto p = ray.GetPoint(temp);
        auto denom = n * n;
        auto l1 = (n * ((b - a) ^ (p - a))) / denom;
        auto l2 = (n * ((c - b) ^ (p - b))) / denom;
        auto l3 = 1.0 - l1 - l2;
        if (l1 >= 0.0 && l1 <= 1.0 && l2 >= 0.0 && l2 <= 1.0 && l3 >= 0.0 && l3 <= 1.0)
        {
            record.Hit = true;
            record.Normal = (n * -ray.Direction > 0.0 ? n.normalised() : -n.normalised());
            record.NormalFlipped = n * ray.Direction > 0.0;
            record.HitPoint = p;
            record.MaterialPtr = materialPtr;
            record.Ray = ray;
            record.T = temp;
        }
    }
    return record;
}
