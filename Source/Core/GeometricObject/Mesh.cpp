#include "Mesh.h"

MeshTriangle::MeshTriangle(Vec3D vertexA, Vec3D vertexB, Vec3D vertexC,
    Vec3D normalA, Vec3D normalB, Vec3D normalC)
    : a(vertexA), b(vertexB), c(vertexC), na(normalA), nb(normalB), nc(normalC)
{
    nf = (b - a) ^ (c - a);
    UpdateBoundingBox();
}

void MeshTriangle::UpdateBoundingBox()
{
    boundingBox = BBox(a, a);
    boundingBox.Merge(BBox(b, b));
    boundingBox.Merge(BBox(c, c));
}

void MeshTriangle::SetSmooth(bool newIsSmooth) { isSmooth = newIsSmooth; }
Vec3D MeshTriangle::getA() { return a; }
Vec3D MeshTriangle::getB() { return b; }
Vec3D MeshTriangle::getC() { return c; }
Vec3D MeshTriangle::getNormal() { return nf.normalised(); }

HitRecord MeshTriangle::Hit(const Ray& ray)
{
    HitRecord record;
    auto temp = ((a - ray.Origin) * nf) / (ray.Direction * nf);
    if (temp > kEpsilon)
    {
        auto p = ray.GetPoint(temp);
        auto denom = nf * nf;
        auto l1 = (nf * ((b - a) ^ (p - a))) / denom;
        auto l2 = (nf * ((c - b) ^ (p - b))) / denom;
        auto l3 = 1.0 - l1 - l2;
        if (l1 >= 0.0 && l1 <= 1.0 && l2 >= 0.0 && l2 <= 1.0 && l3 >= 0.0 && l3 <= 1.0)
        {
            record.Hit = true;
            if (!isSmooth)
            {
                record.Normal = (nf * -ray.Direction > 0.0 ? nf.normalised() : -nf.normalised());
                record.NormalFlipped = nf * ray.Direction > 0.0;
            }
            else
            {
                auto tmpn = nc * l1 + na * l2 + nb * l3;
                record.Normal = (tmpn * -ray.Direction > 0.0 ? tmpn.normalised() : -tmpn.normalised());
                record.NormalFlipped = tmpn * ray.Direction > 0.0;
            }
            record.HitPoint = p;
            record.MaterialPtr = materialPtr;
            record.Ray = ray;
            record.T = temp;
        }
    }
    return record;
}

void Mesh::SetMaterial(std::shared_ptr<Material> newMaterialPtr)
{
    materialPtr = newMaterialPtr;
}

HitRecord Mesh::Hit(const Ray& ray)
{
    HitRecord record = RayTracer::Grid::Hit(ray);
    record.MaterialPtr = materialPtr;
    return record;
}
