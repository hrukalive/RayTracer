/*
  ==============================================================================

    Noise.h
    Created: 13 Nov 2018 8:12:38pm
    Author:  bowen

  ==============================================================================
*/

#pragma once

#include "../../Utility/Utility.h"

#define PERM(x) permTable[(x) & NOISE_TABLE_MASK]
#define INDEX(ix, iy, iz) PERM((ix) + PERM((iy) + PERM(iz)))

class LatticeNoise
{
    Random rand;
    void initValueTable();
protected:
    static const unsigned int permTable[NOISE_TABLE_SIZE];
    FP_TYPE valueTable[NOISE_TABLE_SIZE];
public:
    LatticeNoise();
    virtual ~LatticeNoise() = default;
    virtual FP_TYPE valueNoise(const Point3D p) const = 0;
    FP_TYPE valueFractalSum(const Point3D p, int numOctaves = 4, FP_TYPE fsMin = 0.0, FP_TYPE fsMax = 1.0) const;
    FP_TYPE valueTurbulence(const Point3D p, int numOctaves = 4, FP_TYPE fsMax = 1.0) const;
    FP_TYPE valuefBm(const Point3D p, int numOctaves = 4, FP_TYPE gain = 0.6, FP_TYPE lacunarity = 8, FP_TYPE fsMin = 0.0, FP_TYPE fsMax = 1.0) const;
};

class LinearNoise : public LatticeNoise
{
public:
    virtual ~LinearNoise() = default;
    FP_TYPE valueNoise(const Point3D p) const override;
};

class CubicNoise : public LatticeNoise
{
public:
    virtual ~CubicNoise() = default;
    FP_TYPE valueNoise(const Point3D p) const override;
};
