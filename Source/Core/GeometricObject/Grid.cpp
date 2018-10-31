#include "Grid.h"
#include "BBox.h"

RayTracer::Grid::GridIndex::GridIndex(size_t i, size_t j, size_t k) : i(i), j(j), k(k) {}
bool RayTracer::Grid::GridIndex::operator==(const GridIndex& other) const
{
    return i == other.i && j == other.j && k == other.k;
}

std::size_t RayTracer::Grid::GridIndexHasher::operator()(const GridIndex& key) const
{
    return m3D_e_magicbits(key.i, key.j, key.k);
}

void RayTracer::Grid::Setup()
{
    auto p0 = boundingBox.GetMinPoint();
    auto p1 = boundingBox.GetMaxPoint();
    FP_TYPE wx = p1.x - p0.x, wy = p1.y - p0.y, wz = p1.z - p0.z;
    FP_TYPE m = 2.0;
    FP_TYPE s = pow(wx * wy * wz / objects.size(), 1.0 / 3.0);
    nx = (int)floor(m * wx / s) + 1;
    ny = (int)floor(m * wy / s) + 1;
    nz = (int)floor(m * wz / s) + 1;
    for (auto& obj : objects)
    {
        auto bboxMin = obj->GetBoundingBox().GetMinPoint();
        auto bboxMax = obj->GetBoundingBox().GetMaxPoint();
        size_t imin = clamp((bboxMin.x - p0.x) * nx / wx, 0, nx - 1);
        size_t jmin = clamp((bboxMin.y - p0.y) * ny / wy, 0, ny - 1);
        size_t kmin = clamp((bboxMin.z - p0.z) * nz / wz, 0, nz - 1);
        size_t imax = clamp((bboxMax.x - p0.x) * nx / wx, 0, nx - 1);
        size_t jmax = clamp((bboxMax.y - p0.y) * ny / wy, 0, ny - 1);
        size_t kmax = clamp((bboxMax.z - p0.z) * nz / wz, 0, nz - 1);
        for (size_t k = kmin; k <= kmax; k++)
        {
            for (size_t j = jmin; j <= jmax; j++)
            {
                for (size_t i = imin; i <= imax; i++)
                {
                    GridIndex key(i, j, k);
                    if (cells.find(key) == cells.end())
                    {
                        auto compound = std::make_shared<Compound>(Compound());
                        compound->AddObject(obj);
                        cells[key] = compound;
                    }
                    else
                    {
                        std::dynamic_pointer_cast<Compound>(cells[key])->AddObject(obj);
                    }
                }
            }
        }
    }
}

HitRecord RayTracer::Grid::Hit(const Ray& ray)
{
    auto p0 = boundingBox.GetMinPoint();
    auto p1 = boundingBox.GetMaxPoint();
    FP_TYPE wx = p1.x - p0.x, wy = p1.y - p0.y, wz = p1.z - p0.z;

    FP_TYPE xMin = p0.x, yMin = p0.y, zMin = p0.z, xMax = p1.x, yMax = p1.y, zMax = p1.z;
    FP_TYPE txMin, txMax, tyMin, tyMax, tzMin, tzMax;

    FP_TYPE a = 1.0 / ray.Direction.x;
    txMin = (xMin - ray.Origin.x) * a;
    txMax = (xMax - ray.Origin.x) * a;
    if (a < 0.0)
    {
        std::swap(txMin, txMax);
    }

    FP_TYPE b = 1.0 / ray.Direction.y;
    tyMin = (yMin - ray.Origin.y) * b;
    tyMax = (yMax - ray.Origin.y) * b;
    if (b < 0.0)
    {
        std::swap(tyMin, tyMax);
    }

    FP_TYPE c = 1.0 / ray.Direction.z;
    tzMin = (zMin - ray.Origin.z) * c;
    tzMax = (zMax - ray.Origin.z) * c;
    if (c < 0.0)
    {
        std::swap(tzMin, tzMax);
    }

    FP_TYPE t0, t1;
    if (txMin > tyMin)
    {
        t0 = txMin;
    }
    else
    {
        t0 = tyMin;
    }
    if (tzMin > t0)
    {
        t0 = tzMin;
    }


    if (txMax < tyMax)
    {
        t1 = txMax;
    }
    else
    {
        t1 = tyMax;
    }
    if (tzMax < t1)
    {
        t1 = tzMax;
    }

    if (!(t0 < t1 && t1 > kEpsilon))
    {
        return HitRecord();
    }

    long ix, iy, iz;
    if (boundingBox.Inside(ray.Origin))
    {
        ix = clamp((ray.Origin.x - p0.x) * nx / wx, 0, nx - 1);
        iy = clamp((ray.Origin.y - p0.y) * ny / wy, 0, ny - 1);
        iz = clamp((ray.Origin.z - p0.z) * nz / wz, 0, nz - 1);
    }
    else
    {
        auto p = ray.GetPoint(t0);
        ix = clamp((p.x - p0.x) * nx / wx, 0, nx - 1);
        iy = clamp((p.y - p0.y) * ny / wy, 0, ny - 1);
        iz = clamp((p.z - p0.z) * nz / wz, 0, nz - 1);
    }

    FP_TYPE dtx = (txMax - txMin) / nx, dty = (tyMax - tyMin) / ny, dtz = (tzMax - tzMin) / nz;
    FP_TYPE txNext = txMin + (ix + 1) * dtx, tyNext = tyMin + (iy + 1) * dty, tzNext = tzMin + (iz + 1) * dtz;
    int txStep = 1, tyStep = 1, tzStep = 1;
    long txStop = nx, tyStop = ny, tzStop = nz;

    if (abs(ray.Direction.x) < kEpsilon)
    {
        txNext = kHugeValue;
        txStep = -1;
        txStop = -1;
    }
    else if (ray.Direction.x < 0)
    {
        txNext = txMin + (nx - ix) * dtx;
        txStep = -1;
        txStop = -1;
    }

    if (abs(ray.Direction.y) < kEpsilon)
    {
        tyNext = kHugeValue;
        tyStep = -1;
        tyStop = -1;
    }
    else if (ray.Direction.y < 0)
    {
        tyNext = tyMin + (ny - iy) * dty;
        tyStep = -1;
        tyStop = -1;
    }

    if (abs(ray.Direction.z) < kEpsilon)
    {
        tzNext = kHugeValue;
        tzStep = -1;
        tzStop = -1;
    }
    else if (ray.Direction.z < 0)
    {
        tzNext = tzMin + (nz - iz) * dtz;
        tzStep = -1;
        tzStop = -1;
    }

    while (true)
    {
        auto objPtr = cells.find(GridIndex(ix, iy, iz));
        if (txNext < tyNext && txNext < tzNext)
        {
            if (objPtr != cells.end())
            {
                HitRecord record = (*objPtr).second->Hit(ray);
                if (record.Hit && record.T < txNext)
                {
                    return record;
                }
            }
            txNext += dtx;
            ix += txStep;
            if (ix == txStop)
            {
                return HitRecord();
            }
        }
        else
        {
            if (tyNext < tzNext)
            {
                if (objPtr != cells.end())
                {
                    HitRecord record = (*objPtr).second->Hit(ray);
                    if (record.Hit && record.T < tyNext)
                    {
                        return record;
                    }
                }
                tyNext += dty;
                iy += tyStep;
                if (iy == tyStop)
                {
                    return HitRecord();
                }
            }
            else
            {
                if (objPtr != cells.end())
                {
                    HitRecord record = (*objPtr).second->Hit(ray);
                    if (record.Hit && record.T < tzNext)
                    {
                        return record;
                    }
                }
                tzNext += dtz;
                iz += tzStep;
                if (iz == tzStop)
                {
                    return HitRecord();
                }
            }
        }
    }
    return HitRecord();
}
