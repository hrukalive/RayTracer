#include "Mapping.h"

void LightProbe::setPanoramic(const bool panoramic)
{
    isPanoramic = panoramic;
}

Point2D LightProbe::getTexelCoord(const Point3D hitPoint, const int hres, const int vres) const
{
    FP_TYPE x = hitPoint.x, y = hitPoint.y, z = hitPoint.z;
    FP_TYPE d = sqrt(x * x + y * y);
    FP_TYPE sinBeta = y / d;
    FP_TYPE cosBeta = x / d;
    FP_TYPE alpha = isPanoramic ? acos(-z) : acos(z);
    FP_TYPE r = alpha * INV_PI;
    FP_TYPE u = (1 + r * cosBeta) * 0.5;
    FP_TYPE v = (1 + r * sinBeta) * 0.5;
    return Point2D((hres - 1) * u, (vres - 1) * v);
}
