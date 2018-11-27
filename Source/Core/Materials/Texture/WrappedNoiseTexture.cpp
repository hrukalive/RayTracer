#include "Texture.h"

WrappedNoiseTexture::WrappedNoiseTexture(const std::shared_ptr<LatticeNoise> noisePtr) :
    NoiseTexture(noisePtr) {}

void WrappedNoiseTexture::setExpansion(FP_TYPE newExpansion)
{
    expansion = newExpansion;
}

RGBColor WrappedNoiseTexture::getColor(const HitRecord& record) const
{
    FP_TYPE noise;
    switch (type)
    {
    case NoiseTextureType::FRACTAL_SUM:
        noise = expansion * noisePtr->valueFractalSum(record.HitPoint, numOctaves);
        break;
    case NoiseTextureType::TURBULENCE:
        noise = expansion * noisePtr->valueTurbulence(record.HitPoint, numOctaves);
        break;
    case NoiseTextureType::BROWNIAN:
        noise = expansion * noisePtr->valuefBm(record.HitPoint, numOctaves, gain, lacunarity);
        break;
    }
    FP_TYPE value = noise - floor(noise);
    return color * value;
}
