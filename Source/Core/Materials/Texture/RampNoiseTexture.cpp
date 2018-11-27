#include "Texture.h"

RampNoiseTexture::RampNoiseTexture(const std::shared_ptr<LatticeNoise> noisePtr, const std::shared_ptr<ColorRamp> rampPtr) : 
    NoiseTexture(noisePtr), rampPtr(rampPtr) {}

void RampNoiseTexture::setAmount(FP_TYPE newAmount)
{
    amount = newAmount;
}

RGBColor RampNoiseTexture::getColor(const HitRecord& record) const
{
    FP_TYPE noise;
    switch (type)
    {
    case NoiseTextureType::FRACTAL_SUM:
        noise = noisePtr->valueFractalSum(record.HitPoint, numOctaves, minVal, maxVal);
        break;
    case NoiseTextureType::TURBULENCE:
        noise = noisePtr->valueTurbulence(record.HitPoint, numOctaves, maxVal);
        break;
    case NoiseTextureType::BROWNIAN:
        noise = noisePtr->valuefBm(record.HitPoint, numOctaves, gain, lacunarity, minVal, maxVal);
        break;
    }
    FP_TYPE u = 0.5 * (1 + sin(record.HitPoint.y + amount * noise));
    return rampPtr->getColor(u);
}
