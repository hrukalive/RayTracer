#include "BBox.h"
#include "../Materials/Material.h"

BBox::BBox()
{
    SetBoundingBox(ZERO, ZERO);
}

BBox::BBox(const Point3D min, const Point3D max)
{
    SetBoundingBox(min, max);
}

BBox::~BBox() {}

void BBox::SetBoundingBox(const Point3D min, const Point3D max)
{
    this->bboxMin = min;
    this->bboxMax = max;
    diagLength = (bboxMax - bboxMin).length();
    delta = (diagLength * kBBOXRatio > KBBOXMax ? KBBOXMax : (diagLength * kBBOXRatio));
}

void BBox::Merge(const BBox& other)
{
    SetBoundingBox(Point3D(fmin(bboxMin.x, other.bboxMin.x), fmin(bboxMin.y, other.bboxMin.y), fmin(bboxMin.z, other.bboxMin.z)),
        Point3D(fmax(bboxMax.x, other.bboxMax.x), fmax(bboxMax.y, other.bboxMax.y), fmax(bboxMax.z, other.bboxMax.z)));
}

Point3D BBox::GetMinPoint() 
{ 
    return bboxMin - Point3D(delta, delta, delta); 
}

Point3D BBox::GetMaxPoint() 
{ 
    return bboxMax + Point3D(delta, delta, delta); 
}

Vec3D BBox::GetNormal(const int face) const
{
    switch (face)
    {
    case 0: return -X_DIR;
    case 1: return -Y_DIR;
    case 2: return -Z_DIR;
    case 3: return X_DIR;
    case 4: return Y_DIR;
    case 5: return Z_DIR;
    default: return ZERO;
    }
}

bool BBox::Inside(const Point3D point)
{
    return bboxMin < point && point < bboxMax;
}

HitRecord BBox::Hit(const Ray& ray)
{
    auto& rayOrigin = ray.Origin;
    auto& rayDir = ray.Direction;

    FP_TYPE xMin = bboxMin.x, yMin = bboxMin.y, zMin = bboxMin.z, xMax = bboxMax.x, yMax = bboxMax.y, zMax = bboxMax.z;
    FP_TYPE txMin, txMax, tyMin, tyMax, tzMin, tzMax;

    FP_TYPE a = 1.0 / rayDir.x;
    txMin = (xMin - delta - rayOrigin.x) * a;
    txMax = (xMax + delta - rayOrigin.x) * a;
    if (a < 0.0)
        std::swap(txMin, txMax);

    FP_TYPE b = 1.0 / rayDir.y;
    tyMin = (yMin - delta - rayOrigin.y) * b;
    tyMax = (yMax + delta - rayOrigin.y) * b;
    if (b < 0.0)
        std::swap(tyMin, tyMax);

    FP_TYPE c = 1.0 / rayDir.z;
    tzMin = (zMin - delta - rayOrigin.z) * c;
    tzMax = (zMax + delta - rayOrigin.z) * c;
    if (c < 0.0)
        std::swap(tzMin, tzMax);

    FP_TYPE t0, t1;
    int faceIn, faceOut;
    if (txMin > tyMin)
    {
        t0 = txMin;
        faceIn = (a >= 0.0) ? 0 : 3;
    }
    else
    {
        t0 = tyMin;
        faceIn = (b >= 0.0) ? 1 : 4;
    }
    if (tzMin > t0)
    {
        t0 = tzMin;
        faceIn = (c >= 0.0) ? 2 : 5;
    }

    if (txMax < tyMax)
    {
        t1 = txMax;
        faceOut = (a >= 0.0) ? 3 : 0;
    }
    else
    {
        t1 = tyMax;
        faceOut = (b >= 0.0) ? 4 : 1;
    }
    if (tzMax < t1)
    {
        t1 = tzMax;
        faceOut = (c >= 0.0) ? 5 : 2;
    }

    HitRecord record;
    std::shared_ptr<Material> tmp{ new ConstColor() };

    if (t0 < t1 && t1 > kEpsilon)
    {
        if (t0 > kEpsilon)
        {
            record.Hit = true;
            record.Normal = GetNormal(faceIn);
            record.HitPoint = ray.GetPoint(t0);
            record.MaterialPtr = tmp;
            record.Ray = ray;
            record.T = t0;
        }
        else
        {
            record.Hit = true;
            record.Normal = GetNormal(faceOut);
            record.HitPoint = ray.GetPoint(t1);
            record.MaterialPtr = tmp;
            record.Ray = ray;
            record.T = t1;
        }
    }
    return record;
}
