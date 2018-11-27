#include "Texture.h"

NoiseTexture::NoiseTexture(const std::shared_ptr<LatticeNoise> noisePtr) : 
    noisePtr(noisePtr), color(WHITE), minVal(0.0), maxVal(1.0) {}

void NoiseTexture::setColor(RGBColor newColor)
{
    color = newColor;
}

void NoiseTexture::setRange(FP_TYPE newMin, FP_TYPE newMax)
{
    minVal = newMin;
    maxVal = newMax;
}

void NoiseTexture::setOctaves(int newOctaves)
{
    numOctaves = newOctaves;
}

void NoiseTexture::setGain(FP_TYPE newGain)
{
    gain = newGain;
}

void NoiseTexture::setLacunarity(int newLacunarity)
{
    lacunarity = newLacunarity;
}

void NoiseTexture::setType(NoiseTextureType newType)
{
    type = newType;
}

RGBColor NoiseTexture::getColor(const HitRecord& record) const
{
    FP_TYPE value;
    switch (type)
    {
    case NoiseTextureType::FRACTAL_SUM:
        value = noisePtr->valueFractalSum(record.HitPoint, numOctaves, minVal, maxVal);
        break;
    case NoiseTextureType::TURBULENCE:
        value = noisePtr->valueTurbulence(record.HitPoint, numOctaves, maxVal);
        break;
    case NoiseTextureType::BROWNIAN:
        value = noisePtr->valuefBm(record.HitPoint, numOctaves, gain, lacunarity, minVal, maxVal);
        break;
    }
    return color * value;
}
