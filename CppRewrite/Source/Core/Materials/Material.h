/*
  ==============================================================================

    Material.h
    Created: 28 Sep 2018 11:03:39am
    Author:  Bowen Sun

  ==============================================================================
*/

#pragma once
#include "../Utility.h"
#include "../World.h"
#include "BRDF.h"

class Material
{
public:
	virtual ~Material() = default;
	virtual RGBColor Shade(const HitRecord& record) { return BLACK; }
	virtual RGBColor AreaLightShade(const HitRecord& record) { return BLACK; }
	virtual RGBColor PathShade(const HitRecord& record) { return BLACK; }
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
    virtual ~Matte() {}
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
        RGBColor L = ElemMul(ambientBRDF->rho(record, wo), record.WorldPtr->GetAmbientLightPtr()->L(record));
        auto& lights = record.WorldPtr->GetLights();
        for (int i = 0; i < lights.size(); i++)
        {
            auto wi = -lights[i]->GetDirection(record);
            auto ndotwi = record.Normal * wi;
            if (ndotwi > 0.0)
            {
                L += ElemMul(diffuseBRDF->f(record, wo, wi), lights[i]->L(record) * ndotwi);
            }
        }
        return L;
	}
};
