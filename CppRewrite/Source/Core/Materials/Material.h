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
	virtual RGBColor Shade(const HitRecord& record);
	virtual RGBColor AreaLightShade(const HitRecord& record);
	virtual RGBColor PathShade(const HitRecord& record);
};

class Matte : public Material
{
	std::unique_ptr<Lambertian> ambientBRDF, diffuseBRDF;
public:
	Matte() : Material()
	{
		ambientBRDF.reset(new Lambertian());
		diffuseBRDF.reset(new Lambertian());
	}
	void SetKa(const float ka)
	{
		ambientBRDF->SetKd(ka);
	}
	void SetKd(const float kd)
	{
		diffuseBRDF->SetKd(kd);
	}
	void SetCd(const RGBColor& c)
	{
		ambientBRDF->SetCd(c);
		diffuseBRDF->SetCd(c);
	}
	RGBColor Shade(const HitRecord& record)
	{
		Vec3D wo = -record.Ray.Direction;
		RGBColor L = ambientBRDF->rho(sr, wo) * 
	}
};
