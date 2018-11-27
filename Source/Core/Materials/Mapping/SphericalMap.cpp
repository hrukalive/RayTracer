#include "Mapping.h"

Point2D SphericalMap::getTexelCoord(const Point3D hitPoint, const int hres, const int vres) const
{
    FP_TYPE theta = acos(hitPoint.y);
    FP_TYPE phi = atan2(hitPoint.x, hitPoint.z);
    if (phi < 0.0)
        phi += TWO_PI;
    FP_TYPE u = phi * INV_TWO_PI;
    FP_TYPE v = 1 - theta * INV_PI;
    return Point2D((hres - 1) * u, (vres - 1) * v);
}
