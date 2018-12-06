/*
  ==============================================================================

    Texture.h
    Created: 13 Nov 2018 9:56:02am
    Author:  Bowen Sun

  ==============================================================================
*/

#pragma once

#include "../../Utility/Utility.h"
#include "../Mapping/Mapping.h"
#include "../Noise/Noise.h"

class Texture
{
public:
    virtual ~Texture() = default;
    virtual RGBColor getColor(const HitRecord& record) const { return BLACK; }

    static std::shared_ptr<Texture> parse(StringArray& cmd, std::unordered_map<String, std::shared_ptr<void>>& env);
};

class ConstTexture : public Texture
{
    RGBColor c = BLACK;
public:
    virtual ~ConstTexture() = default;
    void setColor(RGBColor color);
    RGBColor getColor(const HitRecord& record) const override;

    static std::shared_ptr<Texture> parse(StringArray& cmd, std::unordered_map<String, std::shared_ptr<void>>& env);
};

class Checker3D : public Texture
{
    RGBColor color1 = BLACK, color2 = WHITE;
    FP_TYPE size = 10;
public:
    virtual ~Checker3D() = default;
    void setSize(const FP_TYPE newSize);
    void setColor1(const RGBColor newColor1);
    void setColor2(const RGBColor newColor2);
    RGBColor getColor(const HitRecord& record) const override;

    static std::shared_ptr<Texture> parse(StringArray& cmd, std::unordered_map<String, std::shared_ptr<void>>& env);
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
    NoiseTexture(const std::shared_ptr<LatticeNoise> noisePtr);
    virtual ~NoiseTexture() = default;
    void setColor(RGBColor newColor);
    void setRange(FP_TYPE newMin, FP_TYPE newMax);
    void setOctaves(int newOctaves);
    void setGain(FP_TYPE newGain);
    void setLacunarity(int newLacunarity);
    void setType(NoiseTextureType newType);
    RGBColor getColor(const HitRecord& record) const override;

    static std::shared_ptr<Texture> parse(StringArray& cmd, std::unordered_map<String, std::shared_ptr<void>>& env);
};

class WrappedNoiseTexture : public NoiseTexture
{
    FP_TYPE expansion;
public:
    WrappedNoiseTexture(const std::shared_ptr<LatticeNoise> noisePtr);
    virtual ~WrappedNoiseTexture() = default;
    void setExpansion(FP_TYPE newExpansion);
    RGBColor getColor(const HitRecord& record) const override;

    static std::shared_ptr<Texture> parse(StringArray& cmd, std::unordered_map<String, std::shared_ptr<void>>& env);
};

class RampNoiseTexture : public NoiseTexture
{
    FP_TYPE amount = 0;
    std::shared_ptr<ColorRamp> rampPtr;
public:
    RampNoiseTexture(const std::shared_ptr<LatticeNoise> noisePtr, const std::shared_ptr<ColorRamp> rampPtr);
    virtual ~RampNoiseTexture() = default;
    void setAmount(FP_TYPE newAmount);
    RGBColor getColor(const HitRecord& record) const override;

    static std::shared_ptr<Texture> parse(StringArray& cmd, std::unordered_map<String, std::shared_ptr<void>>& env);
};

class TextureInstance : public Texture
{
    Matrix invMatrix = Matrix::identity(4);
    std::shared_ptr<Texture> texturePtr;
public:
    TextureInstance(const std::shared_ptr<Texture> texture);
    virtual ~TextureInstance() = default;
    void setInvMatrix(const Matrix& inv);

    void Translate(const FP_TYPE dx, const FP_TYPE dy, const FP_TYPE dz);
    void Scale(const FP_TYPE a, const FP_TYPE b, const FP_TYPE c);
    void RotateX(const FP_TYPE radian);
    void RotateY(const FP_TYPE radian);
    void RotateZ(const FP_TYPE radian);

    RGBColor getColor(const HitRecord& record) const override;

    static std::shared_ptr<Texture> parse(StringArray& cmd, std::unordered_map<String, std::shared_ptr<void>>& env);
};
