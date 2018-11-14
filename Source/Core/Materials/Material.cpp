/*
  ==============================================================================

    Material.cpp
    Created: 6 Oct 2018 4:38:22pm
    Author:  bowen

  ==============================================================================
*/

#include <algorithm>
#include "Material.h"
#include "../World.h"
#include "../Tracer.h"
#include "../Globals.h"

RGBColor Material::GetLe(const HitRecord& record) { return BLACK; }
RGBColor Material::Shade(const HitRecord& record) { return BLACK; }
RGBColor Material::PathShade(const HitRecord& record) { return BLACK; }

ConstColor::ConstColor(RGBColor color) : c(color) {}
RGBColor ConstColor::Shade(const HitRecord& record)
{
    return c;
}
RGBColor ConstColor::PathShade(const HitRecord& record)
{
    return c;
}

RGBColor NormalShade::Shade(const HitRecord& record)
{
    return (record.Normal + Vec3D(1.0, 1.0, 1.0)) / 2.0;
}
RGBColor NormalShade::PathShade(const HitRecord& record)
{
    return Shade(record);
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
void Matte::SetCd(const std::shared_ptr<Texture> c)
{
    ambientBRDF.SetCd(c);
    diffuseBRDF.SetCd(c);
}
RGBColor Matte::Shade(const HitRecord& record)
{
	Vec3D wo = -record.Ray.Direction;
	RGBColor L = ElemMul(ambientBRDF.rho(record, wo), world->GetAmbientLightPtr()->L(record));
	auto& lights = world->GetLights();
	for (int i = 0; i < lights.size(); i++)
	{
        if (dynamic_cast<const AreaLight*>(lights[i].get()) != nullptr)
        {
            auto light = std::dynamic_pointer_cast<AreaLight>(lights[i]);
            auto samples = light->GetWiAndLGPDF(record);

            for (auto& sample : samples)
            {
                auto samplePoint = sample.first;
                auto wi = sample.second.first;
                auto ndotwi = record.Normal * wi;
                auto ndotwo = record.Normal * wo;
                if (ndotwi > 0.0 && ndotwo > 0.0)
                {
                    Ray shadowRay(record.HitPoint, wi);
                    if (!light->InShadow(shadowRay, samplePoint, record))
                        L += ElemMul(diffuseBRDF.f(record, wi, wo), sample.second.second * ndotwi);
                }
            }
        }
        else
        {
            auto wi = lights[i]->GetDirection(record);
            auto ndotwi = record.Normal * wi;
            auto ndotwo = record.Normal * wo;
            if (ndotwi > 0.0 && ndotwo > 0.0)
            {
                Ray shadowRay(record.HitPoint, wi);
                if (!lights[i]->InShadow(shadowRay, record))
                    L += ElemMul(diffuseBRDF.f(record, wi, wo), lights[i]->L(record) * ndotwi);
            }
        }
	}
	return L;
}

RGBColor Matte::PathShade(const HitRecord& record)
{
    Vec3D wi, wo = -record.Ray.Direction;
    FP_TYPE pdf;
    RGBColor f = diffuseBRDF.sampleF(record, wi, wo, pdf);
    Ray reflected(record.HitPoint, wi);
    return ElemMul(f / pdf, tracer->Trace(reflected, record.Depth + 1) * (record.Normal * wi));
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
void Phong::SetCd(const std::shared_ptr<Texture> c)
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
	RGBColor L = ElemMul(ambientBRDF.rho(record, wo), world->GetAmbientLightPtr()->L(record));
	auto& lights = world->GetLights();
	for (int i = 0; i < lights.size(); i++)
	{
        if (dynamic_cast<const AreaLight*>(lights[i].get()) != nullptr)
        {
            auto light = std::dynamic_pointer_cast<AreaLight>(lights[i]);
            auto samples = light->GetWiAndLGPDF(record);

            for (auto& sample : samples)
            {
                auto samplePoint = sample.first;
                auto wi = sample.second.first;
                auto ndotwi = record.Normal * wi;
                auto ndotwo = record.Normal * wo;
                if (ndotwi > 0.0 && ndotwo > 0.0)
                {
                    Ray shadowRay(record.HitPoint, wi);
                    if (!light->InShadow(shadowRay, samplePoint, record))
                        L += ElemMul(diffuseBRDF.f(record, wi, wo) + specularBRDF.f(record, wi, wo), sample.second.second * ndotwi);
                }
            }
        }
        else
        {
            auto wi = lights[i]->GetDirection(record);
            auto ndotwi = record.Normal * wi;
            auto ndotwo = record.Normal * wo;
            if (ndotwi > 0.0 && ndotwo > 0.0)
            {
                Ray shadowRay(record.HitPoint, wi);
                if (!lights[i]->InShadow(shadowRay, record))
                {
                    L += ElemMul(diffuseBRDF.f(record, wi, wo) + specularBRDF.f(record, wi, wo),
                        lights[i]->L(record) * ndotwi);
                }
            }
        }
	}
	return L;
}

RGBColor Phong::PathShade(const HitRecord& record)
{
    Vec3D wi, wo = -record.Ray.Direction;
    FP_TYPE pdf;
    RGBColor f = diffuseBRDF.sampleF(record, wi, wo, pdf);
    Ray reflected(record.HitPoint, wi);
    return ElemMul(f / pdf, tracer->Trace(reflected, record.Depth + 1) * (record.Normal * wi));
}

RGBColor Emissive::Shade(const HitRecord& record)
{
    if (-record.Normal * record.Ray.Direction > 0.0 && !record.NormalFlipped)
        return ce * std::min((FP_TYPE)1.0, ls);
    else
        return BLACK;
}

RGBColor Emissive::PathShade(const HitRecord& record)
{
    return Shade(record);
}

RGBColor Reflective::Shade(const HitRecord& record)
{
    RGBColor L(Matte::Shade(record));
    Vec3D wo = -record.Ray.Direction;
    Vec3D wi;
    FP_TYPE pdf;
    RGBColor fr = reflectiveBRDF.sampleF(record, wi, wo, pdf);
    Ray reflected(record.HitPoint, wi);
    
    L += ElemMul(fr / pdf, tracer->Trace(reflected, record.Depth + 1) * (record.Normal * wi));
    return L;
}

RGBColor Reflective::PathShade(const HitRecord& record)
{
    return Shade(record);
}

RGBColor GlossyReflector::Shade(const HitRecord& record)
{
    RGBColor L(Matte::Shade(record));
    Vec3D wo = -record.Ray.Direction;

    auto samples = glossyBRDF.GetWiAndF(record, wo);
    for (auto& sample : samples)
    {
        auto wi = sample.first;
        auto frPDF = sample.second;
        Ray reflected(record.HitPoint, wi);
        L += ElemMul(frPDF, tracer->Trace(reflected, record.Depth + 1) * (record.Normal * wi));
    }
    return L;
}

RGBColor GlossyReflector::PathShade(const HitRecord& record)
{
    RGBColor L;
    Vec3D wo = -record.Ray.Direction;
    Vec3D wi;
    FP_TYPE pdf;
    RGBColor fr = glossyBRDF.sampleF(record, wi, wo, pdf);
    Ray reflected(record.HitPoint, wi);
    L += ElemMul(fr / pdf, tracer->Trace(reflected, record.Depth + 1) * (record.Normal * wi));
    return L;
}

RGBColor Transparent::Shade(const HitRecord& record)
{
    RGBColor L(Phong::Shade(record));
    Vec3D wo = -record.Ray.Direction;
    Vec3D wi;
    FP_TYPE pdf;
    RGBColor fr = reflectiveBRDF.sampleF(record, wi, wo, pdf);
    Ray reflected(record.HitPoint, wi);

    if (specularBTDF.tir(record))
        L += tracer->Trace(reflected, record.Depth + 1) * (record.Normal * wi);
    else
    {
        Vec3D wt;
        RGBColor ft = specularBTDF.sampleF(record, wo, wt);
        Ray transmitted(record.HitPoint, wt);
        L += ElemMul(fr, tracer->Trace(reflected, record.Depth + 1) * fabs(record.Normal * wi));
        L += ElemMul(ft, tracer->Trace(transmitted, record.Depth + 1) * fabs(record.Normal * wt));
    }
    return L;
}

RGBColor Transparent::PathShade(const HitRecord& record)
{
    return Shade(record);
}
