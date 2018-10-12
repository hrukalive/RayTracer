/*
  ==============================================================================

    Material.cpp
    Created: 6 Oct 2018 4:38:22pm
    Author:  bowen

  ==============================================================================
*/

#include "Material.h"
#include "../World.h"

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
				L += ElemMul(diffuseBRDF.f(record, wi, wo), lights[i]->L(record) * ndotwi);
			}
		}
	}
	return L;
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
				L += ElemMul(diffuseBRDF.f(record, wi, wo) + specularBRDF.f(record, wi, wo), lights[i]->L(record) * ndotwi);
			}
		}
	}
	return L;
}
