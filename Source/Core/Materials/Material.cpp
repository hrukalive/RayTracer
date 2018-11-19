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
std::pair<bool, RGBColor> Material::PathShade(const HitRecord& record) { return std::make_pair(false, BLACK); }

ConstColor::ConstColor(RGBColor color) : c(color) {}
RGBColor ConstColor::Shade(const HitRecord& record)
{
    return c;
}
std::pair<bool, RGBColor> ConstColor::PathShade(const HitRecord& record)
{
    return std::make_pair(true, c);
}

RGBColor NormalShade::Shade(const HitRecord& record)
{
    return (record.Normal + Vec3D(1.0, 1.0, 1.0)) / 2.0;
}
std::pair<bool, RGBColor> NormalShade::PathShade(const HitRecord& record)
{
    return std::make_pair(true, Shade(record));
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

std::pair<bool, RGBColor> Matte::PathShade(const HitRecord& record)
{
    Vec3D wi, wo = -record.Ray.Direction;
    FP_TYPE pdf;
    RGBColor f = diffuseBRDF.sampleF(record, wi, wo, pdf);
    Ray reflected(record.HitPoint, wi);

    auto traceResult = tracer->Trace(reflected, record.Depth + 1);
    return std::make_pair(traceResult.first, ElemMul(f / pdf, traceResult.second * (record.Normal * wi)));
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

std::pair<bool, RGBColor> Phong::PathShade(const HitRecord& record)
{
    Vec3D wi, wo = -record.Ray.Direction;
    FP_TYPE pdf;
    RGBColor f = diffuseBRDF.sampleF(record, wi, wo, pdf);
    Ray reflected(record.HitPoint, wi);

    auto traceResult = tracer->Trace(reflected, record.Depth + 1);
    return std::make_pair(traceResult.first, ElemMul(f / pdf, traceResult.second * (record.Normal * wi)));
}

RGBColor Emissive::Shade(const HitRecord& record)
{
    if (-record.Normal * record.Ray.Direction > 0.0 && !record.NormalFlipped)
        return ce * std::min((FP_TYPE)1.0, ls);
    else
        return BLACK;
}

std::pair<bool, RGBColor> Emissive::PathShade(const HitRecord& record)
{
    if (-record.Normal * record.Ray.Direction > 0.0 && !record.NormalFlipped)
        return std::make_pair(true, ce * ls);
    else
        return std::make_pair(true, BLACK);
}

RGBColor Reflective::Shade(const HitRecord& record)
{
    Vec3D wo = -record.Ray.Direction;
    Vec3D wi;
    FP_TYPE pdf;
    RGBColor fr = reflectiveBRDF.sampleF(record, wi, wo, pdf);
    Ray reflected(record.HitPoint, wi);
    
    return ElemMul(fr / pdf, tracer->Trace(reflected, record.Depth + 1).second * (record.Normal * wi));
}

std::pair<bool, RGBColor> Reflective::PathShade(const HitRecord& record)
{
    Vec3D wo = -record.Ray.Direction;
    Vec3D wi;
    FP_TYPE pdf;
    RGBColor fr = reflectiveBRDF.sampleF(record, wi, wo, pdf);
    Ray reflected(record.HitPoint, wi);

    auto traceResult = tracer->Trace(reflected, record.Depth + 1);
    return std::make_pair(traceResult.first, ElemMul(fr / pdf, traceResult.second * (record.Normal * wi)));
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
        L += ElemMul(frPDF, tracer->Trace(reflected, record.Depth + 1).second * (record.Normal * wi));
    }
    return L;
}

std::pair<bool, RGBColor> GlossyReflector::PathShade(const HitRecord& record)
{
    RGBColor L;
    Vec3D wo = -record.Ray.Direction;
    Vec3D wi;
    FP_TYPE pdf;
    RGBColor fr = glossyBRDF.sampleF(record, wi, wo, pdf);
    Ray reflected(record.HitPoint, wi);
    auto traceResult = tracer->Trace(reflected, record.Depth + 1);
    L += ElemMul(fr / pdf, traceResult.second * (record.Normal * wi));
    return std::make_pair(traceResult.first, L);
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
        L += tracer->Trace(reflected, record.Depth + 1).second * (record.Normal * wi);
    else
    {
        Vec3D wt;
        RGBColor ft = specularBTDF.sampleF(record, wo, wt);
        Ray transmitted(record.HitPoint, wt);
        L += ElemMul(fr, tracer->Trace(reflected, record.Depth + 1).second * fabs(record.Normal * wi));
        L += ElemMul(ft, tracer->Trace(transmitted, record.Depth + 1).second * fabs(record.Normal * wt));
    }
    return L;
}

std::pair<bool, RGBColor> Transparent::PathShade(const HitRecord& record)
{
    RGBColor L;
    Vec3D wo = -record.Ray.Direction;
    Vec3D wi;
    FP_TYPE pdf;
    RGBColor fr = reflectiveBRDF.sampleF(record, wi, wo, pdf);
    Ray reflected(record.HitPoint, wi);
    bool hasTransport = false;

    if (specularBTDF.tir(record))
    {
        auto traceResult = tracer->Trace(reflected, record.Depth + 1);
        L += traceResult.second * (record.Normal * wi);
        hasTransport |= traceResult.first;
    }
    else
    {
        Vec3D wt;
        RGBColor ft = specularBTDF.sampleF(record, wo, wt);
        Ray transmitted(record.HitPoint, wt);
        auto traceResult = tracer->Trace(reflected, record.Depth + 1);
        L += ElemMul(fr, traceResult.second * fabs(record.Normal * wi));
        hasTransport |= traceResult.first;
        traceResult = tracer->Trace(transmitted, record.Depth + 1);
        L += ElemMul(ft, traceResult.second * fabs(record.Normal * wt));
        hasTransport |= traceResult.first;
    }
    return std::make_pair(hasTransport, L);
}
