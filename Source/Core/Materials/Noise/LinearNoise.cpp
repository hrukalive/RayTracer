#include "Noise.h"

FP_TYPE LinearNoise::valueNoise(const Point3D p) const
{
    int ix = floor(p.x), iy = floor(p.y), iz = floor(p.z);
    FP_TYPE fx = p.x - ix, fy = p.y - iy, fz = p.z - iz;
    FP_TYPE d[2][2][2];
    for (int k = 0; k <= 1; k++)
        for (int j = 0; j <= 1; j++)
            for (int i = 0; i <= 1; i++)
                d[k][j][i] = valueTable[INDEX(ix + i, iy + j, iz + k)];
    FP_TYPE x0 = lerp(fx, d[0][0][0], d[0][0][1]);
    FP_TYPE x1 = lerp(fx, d[0][1][0], d[0][1][1]);
    FP_TYPE x2 = lerp(fx, d[1][0][0], d[1][0][1]);
    FP_TYPE x3 = lerp(fx, d[1][1][0], d[1][1][1]);
    FP_TYPE y0 = lerp(fy, x0, x1), y1 = lerp(fy, x2, x3);
    return lerp(fz, y0, y1);
}
