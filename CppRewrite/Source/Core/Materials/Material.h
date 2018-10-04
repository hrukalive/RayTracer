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
	Lambertian ambientBRDF, diffuseBRDF;
public:
	Matte() : Material()
	{
	}
    virtual ~Matte() {}
	void SetKa(const float ka)
	{
		ambientBRDF.SetKd(ka);
	}
	void SetKd(const float kd)
	{
		diffuseBRDF.SetKd(kd);
	}
	void SetCd(const RGBColor& c)
	{
		ambientBRDF.SetCd(c);
		diffuseBRDF.SetCd(c);
	}
	RGBColor Shade(const HitRecord& record)
	{
		Vec3D wo = -record.Ray.Direction;
        RGBColor L = ElemMul(ambientBRDF.rho(record, wo), record.WorldPtr->GetAmbientLightPtr()->L(record));
        auto& lights = record.WorldPtr->GetLights();
        for (int i = 0; i < lights.size(); i++)
        {
            auto wi = lights[i]->GetDirection(record);
            auto ndotwi = record.Normal * wi;
            auto ndotwo = record.Normal * wo;
            if (ndotwi > 0.0 && ndotwo > 0.0)
            {
                Ray shadowRay(record.HitPoint, wi);
                if (!lights[i]->InShadow(shadowRay, record))
                {
                    L += ElemMul(diffuseBRDF.f(record, wi, wo), lights[i]->L(record) * ndotwi);
                }
            }
        }
        return L;
	}
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
    void SetKa(const float ka)
    {
        ambientBRDF.SetKd(ka);
    }
    void SetKd(const float kd)
    {
        diffuseBRDF.SetKd(kd);
    }
    void SetKs(const float ks)
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
    void SetE(const float exp)
    {
        specularBRDF.SetE(exp);
    }
    RGBColor Shade(const HitRecord& record)
    {
        Vec3D wo = -record.Ray.Direction;
        RGBColor L = ElemMul(ambientBRDF.rho(record, wo), record.WorldPtr->GetAmbientLightPtr()->L(record));
        auto& lights = record.WorldPtr->GetLights();
        for (int i = 0; i < lights.size(); i++)
        {
            auto wi = lights[i]->GetDirection(record);
            auto ndotwi = record.Normal * wi;
            auto ndotwo = record.Normal * wo;
            if (ndotwi > 0.0 && ndotwo > 0.0)
            {
                Ray shadowRay(record.HitPoint, wi);
                if (!lights[i]->InShadow(shadowRay, record))
                {
                    L += ElemMul(diffuseBRDF.f(record, wi, wo) + specularBRDF.f(record, wi, wo), lights[i]->L(record) * ndotwi);
                }
            }
        }
        return L;
    }
};
