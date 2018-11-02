/*
  ==============================================================================

    Material.h
    Created: 28 Sep 2018 11:03:39am
    Author:  Bowen Sun

  ==============================================================================
*/

#pragma once
#include "../Utility.h"
#include "BRDF.h"

class Material
{
public:
	virtual ~Material() = default;
    virtual RGBColor GetLe(const HitRecord& record);
    virtual RGBColor Shade(const HitRecord& record);
    virtual RGBColor AreaLightShade(const HitRecord& record);
    virtual RGBColor PathShade(const HitRecord& record);
};

class ConstColor : public Material
{
	RGBColor c = RED;
public:
    ConstColor() = default;
    ConstColor(RGBColor color);
    virtual ~ConstColor() = default;
    RGBColor Shade(const HitRecord& record) override;
};

class NormalShade : public Material
{
public:
	virtual ~NormalShade() = default;
    RGBColor Shade(const HitRecord& record) override;
};

class Matte : public Material
{
	Lambertian ambientBRDF, diffuseBRDF;
public:
    virtual ~Matte() = default;
    void SetKa(const FP_TYPE ka);
    void SetKd(const FP_TYPE kd);
    void SetCd(const RGBColor& c);
	RGBColor Shade(const HitRecord& record) override;
};

class Phong : public Material
{
    Lambertian ambientBRDF, diffuseBRDF;
    GlossySpecular specularBRDF;
public:
    virtual ~Phong() = default;
    void SetKa(const FP_TYPE ka);
    void SetKd(const FP_TYPE kd);
    void SetKs(const FP_TYPE ks);
    void SetCd(const RGBColor& c);
    void SetCs(const RGBColor& c);
    void SetE(const FP_TYPE exp);
	RGBColor Shade(const HitRecord& record) override;
};

class Emissive : public Material
{
    float ls;
    RGBColor ce;
public:
    virtual ~Emissive() = default;
    void SetLs(const FP_TYPE newLs)
    {
        ls = newLs;
    }
    void SetCe(const RGBColor& newCe)
    {
        ce = newCe;
    }
    virtual RGBColor GetLe(const HitRecord& record) override
    {
        return ce * ls;
    }
    RGBColor Shade(const HitRecord& record) override;
};

class Reflective : public Phong
{
    PerfectSpecular reflectiveBRDF;
public:
    virtual ~Reflective() = default;
    void SetKr(const FP_TYPE newKr)
    {
        reflectiveBRDF.SetKr(newKr);
    }
    void SetCr(const RGBColor& newCr)
    {
        reflectiveBRDF.SetCr(newCr);
    }
    RGBColor Shade(const HitRecord& record) override;
};

class GlossyReflector : public Phong
{
    GlossySpecular glossyBRDF;
public:
    virtual ~GlossyReflector() = default;
    void SetKs(const FP_TYPE newKs)
    {
        glossyBRDF.SetKs(newKs);
    }
    void SetE(const FP_TYPE newE)
    {
        glossyBRDF.SetE(newE);
    }
    RGBColor Shade(const HitRecord& record) override;
};
