/*
  ==============================================================================

    Material.cpp
    Created: 6 Oct 2018 4:38:22pm
    Author:  bowen

  ==============================================================================
*/

#include "Material.h"
#include "../World.h"

RGBColor Material::Shade(const HitRecord& record) { return BLACK; }
RGBColor Material::AreaLightShade(const HitRecord& record) { return BLACK; }
RGBColor Material::PathShade(const HitRecord& record) { return BLACK; }

ConstColor::ConstColor(RGBColor color) : c(color) {}
RGBColor ConstColor::Shade(const HitRecord& record)
{
    return c;
}

RGBColor NormalShade::Shade(const HitRecord& record)
{
    return (record.Normal + Vec3D(1.0, 1.0, 1.0)) / 2.0;
}


void Matte::SetKa(const FP_TYPE ka)
{
    ambientBRDF.SetKd(ka);
}
void Matte::SetKd(const FP_TYPE kd)
{
    diffuseBRDF.SetKd(kd);
}
void Matte::SetCd(const RGBColor& c)
{
    ambientBRDF.SetCd(c);
    diffuseBRDF.SetCd(c);
}
RGBColor Matte::Shade(const HitRecord& record)
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
                if (dynamic_cast<const AreaLight*>(lights[i].get()) != nullptr)
                {
                    auto light = std::dynamic_pointer_cast<AreaLight>(lights[i]);
                    L += ElemMul(diffuseBRDF.f(record, wi, wo),
                        light->L(record) * light->G(record) * ndotwi / light->pdf(record));
                }
                else
                    L += ElemMul(diffuseBRDF.f(record, wi, wo), lights[i]->L(record) * ndotwi);
			}
		}
	}
	return L;
}

void Phong::SetKa(const FP_TYPE ka)
{
    ambientBRDF.SetKd(ka);
}
void Phong::SetKd(const FP_TYPE kd)
{
    diffuseBRDF.SetKd(kd);
}
void Phong::SetKs(const FP_TYPE ks)
{
    specularBRDF.SetKs(ks);
}
void Phong::SetCd(const RGBColor& c)
{
    ambientBRDF.SetCd(c);
    diffuseBRDF.SetCd(c);
}
void Phong::SetCs(const RGBColor& c)
{
    specularBRDF.SetCs(c);
}
void Phong::SetE(const FP_TYPE exp)
{
    specularBRDF.SetE(exp);
}
RGBColor Phong::Shade(const HitRecord& record)
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
                if (dynamic_cast<const AreaLight*>(lights[i].get()) != nullptr)
                {
                    auto light = std::dynamic_pointer_cast<AreaLight>(lights[i]);
                    L += ElemMul(diffuseBRDF.f(record, wi, wo) + specularBRDF.f(record, wi, wo),
                        light->L(record) * light->G(record) * ndotwi / light->pdf(record));
                }
                else
				    L += ElemMul(diffuseBRDF.f(record, wi, wo) + specularBRDF.f(record, wi, wo), 
                        lights[i]->L(record) * ndotwi);
			}
		}
	}
	return L;
}

RGBColor Emissive::Shade(const HitRecord& record)
{
    if (-record.Normal * record.Ray.Direction > 0.0)
        return ce * ls;
    else
        return BLACK;
}
