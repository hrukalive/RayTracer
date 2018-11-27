#include "Noise.h"

void LatticeNoise::initValueTable()
{
    for (int i = 0; i < NOISE_TABLE_SIZE; i++)
        valueTable[i] = 2.0 * rand.nextDouble() - 1.0;
}

LatticeNoise::LatticeNoise() { initValueTable(); }

FP_TYPE LatticeNoise::valueFractalSum(const Point3D p, int numOctaves, FP_TYPE fsMin, FP_TYPE fsMax) const
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

FP_TYPE LatticeNoise::valueTurbulence(const Point3D p, int numOctaves, FP_TYPE fsMax) const
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

FP_TYPE LatticeNoise::valuefBm(const Point3D p, int numOctaves, FP_TYPE gain, FP_TYPE lacunarity, FP_TYPE fsMin, FP_TYPE fsMax) const
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
