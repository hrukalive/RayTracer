#include "Noise.h"

FP_TYPE CubicNoise::valueNoise(const Point3D p) const
{
    int ix = floor(p.x), iy = floor(p.y), iz = floor(p.z);
    FP_TYPE fx = p.x - ix, fy = p.y - iy, fz = p.z - iz;
    FP_TYPE xps[4], yps[4], zps[4];
    for (int k = -1; k <= 2; k++)
    {
        for (int j = -1; j <= 2; j++)
        {
            for (int i = -1; i <= 2; i++)
                xps[i + 1] = valueTable[INDEX(ix + i, iy + j, iz + k)];
            yps[j + 1] = cubicSpline(fx, xps[0], xps[1], xps[2], xps[3]);
        }
        zps[k + 1] = cubicSpline(fy, yps[0], yps[1], yps[2], yps[3]);
    }
    return clamp(cubicSpline(fz, zps[0], zps[1], zps[2], zps[3]), (FP_TYPE)-1.0, (FP_TYPE)1.0);
}
