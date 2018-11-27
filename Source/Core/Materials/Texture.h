/*
  ==============================================================================

    Texture.h
    Created: 13 Nov 2018 9:56:02am
    Author:  Bowen Sun

  ==============================================================================
*/

#pragma once

#include "../Utility/Utility.h"
#include "Mapping.h"
#include "Noise.h"

class Texture
{
public:
    virtual ~Texture() = default;
    virtual RGBColor getColor(const HitRecord& record) const { return BLACK; }
};

class ConstTexture : public Texture
{
    RGBColor c = BLACK;
public:
    virtual ~ConstTexture() = default;
    void setColor(RGBColor color)
    {
        c = color;
    }
    RGBColor getColor(const HitRecord& record) const override
    {
        return c;
    }
};

class Checker3D : public Texture
{
    RGBColor color1 = BLACK, color2 = WHITE;
    FP_TYPE size = 10;
public:
    virtual ~Checker3D() = default;
    void setSize(const FP_TYPE newSize)
    {
        size = newSize;
    }
    void setColor1(const RGBColor newColor1)
    {
        color1 = newColor1;
    }
    void setColor2(const RGBColor newColor2)
    {
        color2 = newColor2;
    }
    RGBColor getColor(const HitRecord& record) const override
    {
        FP_TYPE x = record.HitPoint.x - kEpsilon, y = record.HitPoint.y - kEpsilon, z = record.HitPoint.z - kEpsilon;
        if (((int)floor(x / size) + (int)floor(y / size) + (int)floor(z / size)) % 2 == 0)
            return color1;
        else
            return color2;
    }
};

class PlaneChecker : public Texture
{
    RGBColor color1 = RED, color2 = WHITE, outlineColor = BLACK;
    FP_TYPE size = 5, outlineWidth = 3;
public:
    virtual ~PlaneChecker() = default;
    void setSize(const FP_TYPE newSize)
    {
        size = newSize;
    }
    void setColor1(const RGBColor newColor1)
    {
        color1 = newColor1;
    }
    void setColor2(const RGBColor newColor2)
    {
        color2 = newColor2;
    }
    void setOutlineColor(const RGBColor newOutlineColor)
    {
        outlineColor = newOutlineColor;
    }
    RGBColor getColor(const HitRecord& record) const override
    {
        FP_TYPE x = record.HitPoint.x, z = record.HitPoint.z;
        int ix = floor(x / size), iz = floor(z / size);
        FP_TYPE fx = x / size - ix, fz = z / size - iz;
        FP_TYPE width = 0.5 * outlineWidth / size;
        bool inOutline = (fx < width || fx > 1 - width) || (fz < width || fz > 1 - width);

        if (!inOutline)
        {
            if ((ix + iz) % 2 == 0)
                return color1;
            else
                return color2;
        }
        return outlineColor;
    }
};

enum NoiseTextureType
{
    FRACTAL_SUM,
    TURBULENCE,
    BROWNIAN
};

class NoiseTexture : public Texture
{
protected:
    std::shared_ptr<LatticeNoise> noisePtr;
    RGBColor color;
    FP_TYPE minVal, maxVal;
    int numOctaves = 6;
    FP_TYPE gain = 0.5, lacunarity = 2;
    NoiseTextureType type = NoiseTextureType::BROWNIAN;
public:
    NoiseTexture(const std::shared_ptr<LatticeNoise> noisePtr) : noisePtr(noisePtr), color(WHITE), minVal(0.0), maxVal(1.0) {}
    virtual ~NoiseTexture() = default;
    void setColor(RGBColor newColor)
    {
        color = newColor;
    }
    void setRange(FP_TYPE newMin, FP_TYPE newMax)
    {
        minVal = newMin;
        maxVal = newMax;
    }
    void setOctaves(int newOctaves)
    {
        numOctaves = newOctaves;
    }
    void setGain(FP_TYPE newGain)
    {
        gain = newGain;
    }
    void setLacunarity(int newLacunarity)
    {
        lacunarity = newLacunarity;
    }
    void setType(NoiseTextureType newType)
    {
        type = newType;
    }
    RGBColor getColor(const HitRecord& record) const override
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
};

class WrappedNoiseTexture : public NoiseTexture
{
    FP_TYPE expansion;
public:
    WrappedNoiseTexture(const std::shared_ptr<LatticeNoise> noisePtr) : NoiseTexture(noisePtr) {}
    virtual ~WrappedNoiseTexture() = default;
    void setExpansion(FP_TYPE newExpansion)
    {
        expansion = newExpansion;
    }
    RGBColor getColor(const HitRecord& record) const override
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
};

class RampNoiseTexture : public NoiseTexture
{
    FP_TYPE amount = 0;
    std::shared_ptr<ColorRamp> rampPtr;
public:
    RampNoiseTexture(const std::shared_ptr<LatticeNoise> noisePtr, const std::shared_ptr<ColorRamp> rampPtr) : NoiseTexture(noisePtr), rampPtr(rampPtr) {}
    virtual ~RampNoiseTexture() = default;
    void setAmount(FP_TYPE newAmount)
    {
        amount = newAmount;
    }
    RGBColor getColor(const HitRecord& record) const override
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
};

class TextureInstance : public Texture
{
    Matrix invMatrix = Matrix::identity(4);
    std::shared_ptr<Texture> texturePtr;
public:
    TextureInstance(const std::shared_ptr<Texture> texture) : texturePtr(texture) {}
    virtual ~TextureInstance() = default;
    void setInvMatrix(const Matrix& inv)
    {
        invMatrix = inv;
    }

    void Translate(const FP_TYPE dx, const FP_TYPE dy, const FP_TYPE dz)
    {
        auto im = Matrix::identity(4);
        im(0, 3) = -dx;
        im(1, 3) = -dy;
        im(2, 3) = -dz;
        invMatrix = invMatrix * im;
    }

    void Scale(const FP_TYPE a, const FP_TYPE b, const FP_TYPE c)
    {
        auto im = Matrix::identity(4);
        im(0, 0) = 1.0 / a;
        im(1, 1) = 1.0 / b;
        im(2, 2) = 1.0 / c;
        invMatrix = invMatrix * im;
    }

    void RotateX(const FP_TYPE radian)
    {
        auto im = Matrix::identity(4);
        im(1, 1) = cos(radian);
        im(2, 2) = cos(radian);
        im(1, 2) = sin(radian);
        im(2, 1) = -sin(radian);
        invMatrix = invMatrix * im;
    }

    void RotateY(const FP_TYPE radian)
    {
        auto im = Matrix::identity(4);
        im(0, 0) = cos(radian);
        im(2, 2) = cos(radian);
        im(0, 2) = -sin(radian);
        im(2, 0) = sin(radian);
        invMatrix = invMatrix * im;
    }

    void RotateZ(const FP_TYPE radian)
    {
        auto im = Matrix::identity(4);
        im(0, 0) = cos(radian);
        im(1, 1) = cos(radian);
        im(0, 1) = sin(radian);
        im(1, 0) = -sin(radian);
        invMatrix = invMatrix * im;
    }

    RGBColor getColor(const HitRecord& record) const override
    {
        HitRecord tmp = record;
        tmp.HitPoint = MatrixMulVector(invMatrix, tmp.HitPoint);
        return texturePtr->getColor(tmp);
    }
};
