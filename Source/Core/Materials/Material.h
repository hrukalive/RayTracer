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
	virtual RGBColor Shade(const HitRecord& record) { return BLACK; }
	virtual RGBColor AreaLightShade(const HitRecord& record) { return BLACK; }
	virtual RGBColor PathShade(const HitRecord& record) { return BLACK; }
};

class ConstColor : public Material
{
	RGBColor c = RED;
public:
	ConstColor() {}
	ConstColor(RGBColor color) : c(color) {}
	virtual ~ConstColor() {}
	RGBColor Shade(const HitRecord& record) override
	{
		return c;
	}
};

class NormalShade : public Material
{
public:
	virtual ~NormalShade() {}
	RGBColor Shade(const HitRecord& record) override
	{
		return (record.Normal + Vec3D(1.0, 1.0, 1.0)) / 2.0;
	}
};

class Matte : public Material
{
	Lambertian ambientBRDF, diffuseBRDF;
public:
    virtual ~Matte() {}
	void SetKa(const FP_TYPE ka)
	{
		ambientBRDF.SetKd(ka);
	}
	void SetKd(const FP_TYPE kd)
	{
		diffuseBRDF.SetKd(kd);
	}
	void SetCd(const RGBColor& c)
	{
		ambientBRDF.SetCd(c);
		diffuseBRDF.SetCd(c);
	}
	RGBColor Shade(const HitRecord& record) override;
};

class Phong : public Material
{
    Lambertian ambientBRDF, diffuseBRDF;
    GlossySpecular specularBRDF;
public:
    Phong() : Material()
    {
    }
    virtual ~Phong() {}
    void SetKa(const FP_TYPE ka)
    {
        ambientBRDF.SetKd(ka);
    }
    void SetKd(const FP_TYPE kd)
    {
        diffuseBRDF.SetKd(kd);
    }
    void SetKs(const FP_TYPE ks)
    {
        specularBRDF.SetKs(ks);
    }
    void SetCd(const RGBColor& c)
    {
        ambientBRDF.SetCd(c);
        diffuseBRDF.SetCd(c);
    }
    void SetCs(const RGBColor& c)
    {
        specularBRDF.SetCs(c);
    }
    void SetE(const FP_TYPE exp)
    {
        specularBRDF.SetE(exp);
    }
	RGBColor Shade(const HitRecord& record) override;
};
