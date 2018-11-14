/*
  ==============================================================================

    Noise.h
    Created: 13 Nov 2018 8:12:38pm
    Author:  bowen

  ==============================================================================
*/

#pragma once

#include "../Utility.h"

#define PERM(x) permTable[(x) & NOISE_TABLE_MASK]
#define INDEX(ix, iy, iz) PERM((ix) + PERM((iy) + PERM(iz)))

class LatticeNoise
{
    Random rand;
    void initValueTable()
    {
        for (int i = 0; i < NOISE_TABLE_SIZE; i++)
            valueTable[i] = 2.0 * rand.nextDouble() - 1.0;
    }
protected:
    static const unsigned int permTable[NOISE_TABLE_SIZE];
    FP_TYPE valueTable[NOISE_TABLE_SIZE];
public:
    LatticeNoise() { initValueTable(); }
    virtual ~LatticeNoise() = default;
    virtual FP_TYPE valueNoise(const Point3D p) const = 0;
    FP_TYPE valueFractalSum(const Point3D p, int numOctaves = 4, FP_TYPE fsMin = 0.0, FP_TYPE fsMax = 1.0) const
    {
        FP_TYPE amplitude = 1.0, frequency = 1.0, fractalSum = 0.0;
        for (int i = 0; i < numOctaves; i++)
        {
            fractalSum += amplitude * valueNoise(p * frequency);
            amplitude *= 0.5;
            frequency *= 2.0;
        }
        fractalSum = (fractalSum - fsMin) / (fsMax - fsMin);
        return fractalSum;
    }
    FP_TYPE valueTurbulence(const Point3D p, int numOctaves = 4, FP_TYPE fsMax = 1.0) const
    {
        FP_TYPE amplitude = 1.0, frequency = 1.0, turbulence = 0.0;
        for (int i = 0; i < numOctaves; i++)
        {
            turbulence += amplitude * abs(valueNoise(p * frequency));
            amplitude *= 0.5;
            frequency *= 2.0;
        }
        turbulence /= fsMax;
        return turbulence;
    }
    FP_TYPE valuefBm(const Point3D p, int numOctaves = 4, FP_TYPE fsMin = 0.0, FP_TYPE fsMax = 1.0, FP_TYPE gain = 0.6, FP_TYPE lacunarity = 8) const
    {
        FP_TYPE amplitude = 1.0, frequency = 1.0, fBm = 0.0;
        for (int i = 0; i < numOctaves; i++)
        {
            fBm += amplitude * valueNoise(p * frequency);
            amplitude *= gain;
            frequency *= lacunarity;
        }
        fBm = (fBm - fsMin) / (fsMax - fsMin);
        return fBm;
    }
};

class LinearNoise : public LatticeNoise
{
public:
    virtual ~LinearNoise() = default;
    FP_TYPE valueNoise(const Point3D p) const override
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
};

class CubicNoise : public LatticeNoise
{
public:
    virtual ~CubicNoise() = default;
    FP_TYPE valueNoise(const Point3D p) const override
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
};
