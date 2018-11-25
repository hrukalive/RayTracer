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
RGBColor Material::PhotonShade(const HitRecord& record) { return BLACK; }

ConstColor::ConstColor(RGBColor color) : c(color) {}
RGBColor ConstColor::Shade(const HitRecord& record)
{
    return c;
}
std::pair<bool, RGBColor> ConstColor::PathShade(const HitRecord& record)
{
    return std::make_pair(true, c);
}
RGBColor ConstColor::PhotonShade(const HitRecord& record)
{
    return c;
}

RGBColor NormalShade::Shade(const HitRecord& record)
{
    return (record.Normal + Vec3D(1.0, 1.0, 1.0)) / 2.0;
}
std::pair<bool, RGBColor> NormalShade::PathShade(const HitRecord& record)
{
    return std::make_pair(true, Shade(record));
}
RGBColor NormalShade::PhotonShade(const HitRecord& record)
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
            RGBColor tmpL;

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
                        tmpL += ElemMul(diffuseBRDF.f(record, wi, wo), sample.second.second * ndotwi);
                }
            }
            L += tmpL / samples.size();
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

RGBColor Matte::PhotonShade(const HitRecord& record)
{
    float irrad[3], pos[3], normal[3];
    pos[0] = record.HitPoint.x; pos[1] = record.HitPoint.y; pos[2] = record.HitPoint.z;
    normal[0] = record.Normal.x; normal[1] = record.Normal.y; normal[2] = record.Normal.z;
    autoIrradianceEstimate(balancedPhotonMap, irrad, pos, normal, N_PHOTON);
    return RGBColor(irrad[0], irrad[1], irrad[2]);
}

void Matte::PhotonMapping(const HitRecord& record)
{
    auto d = diffuseBRDF.GetCd(record);
    auto pd = std::max(d.x, std::max(d.y, d.z));
    Random rand;

    Vec3D wi, wo = -record.Ray.Direction;
    FP_TYPE pdf;
    RGBColor f = diffuseBRDF.sampleF(record, wi, wo, pdf);
    Ray reflected(record.HitPoint, wi, ElemMul(f, record.Ray.Power / pd));
    if (rand.nextDouble() < pd)
        tracer->Trace(reflected, record.Depth + 1);

    float power[3]{ (float)(f.x * record.Ray.Power.x), (float)(f.y * record.Ray.Power.y), (float)(f.z * record.Ray.Power.z) };
    float pos[3]{ (float)record.HitPoint.x, (float)record.HitPoint.y, (float)record.HitPoint.z };
    float dir[3]{ (float)record.Ray.Direction.x, (float)record.Ray.Direction.y, (float)record.Ray.Direction.z };
    photonMapLock.enter();
    storePhoton(photonMap, power, pos, dir);
    photonMapLock.exit();
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
            RGBColor tmpL;

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
                        tmpL += ElemMul(diffuseBRDF.f(record, wi, wo) + specularBRDF.f(record, wi, wo), sample.second.second * ndotwi);
                }
            }
            L += tmpL / samples.size();
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

RGBColor Phong::PhotonShade(const HitRecord& record)
{
    float irrad[3], pos[3], normal[3];
    pos[0] = record.HitPoint.x; pos[1] = record.HitPoint.y; pos[2] = record.HitPoint.z;
    normal[0] = record.Normal.x; normal[1] = record.Normal.y; normal[2] = record.Normal.z;
    autoIrradianceEstimate(balancedPhotonMap, irrad, pos, normal, N_PHOTON);
    return RGBColor(irrad[0], irrad[1], irrad[2]);
}

void Phong::PhotonMapping(const HitRecord& record)
{
    auto d = diffuseBRDF.GetCd(record);
    auto pd = std::max(d.x, std::max(d.y, d.z));
    Random rand;

    Vec3D wi, wo = -record.Ray.Direction;
    FP_TYPE pdf;
    RGBColor f = diffuseBRDF.sampleF(record, wi, wo, pdf);
    Ray reflected(record.HitPoint, wi, ElemMul(f, record.Ray.Power / pd));
    if (rand.nextDouble() < pd)
        tracer->Trace(reflected, record.Depth + 1);

    float power[3]{ (float)(f.x * record.Ray.Power.x), (float)(f.y * record.Ray.Power.y), (float)(f.z * record.Ray.Power.z) };
    float pos[3]{ (float)record.HitPoint.x, (float)record.HitPoint.y, (float)record.HitPoint.z };
    float dir[3]{ (float)record.Ray.Direction.x, (float)record.Ray.Direction.y, (float)record.Ray.Direction.z };
    photonMapLock.enter();
    storePhoton(photonMap, power, pos, dir);
    photonMapLock.exit();
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

RGBColor Emissive::PhotonShade(const HitRecord& record)
{
    if (-record.Normal * record.Ray.Direction > 0.0 && !record.NormalFlipped)
        return ce * std::min((FP_TYPE)1.0, ls);
    else
        return BLACK;
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

RGBColor Reflective::PhotonShade(const HitRecord& record)
{
    Vec3D wo = -record.Ray.Direction;
    Vec3D wi;
    FP_TYPE pdf;
    RGBColor fr = reflectiveBRDF.sampleF(record, wi, wo, pdf);
    Ray reflected(record.HitPoint, wi);

    return ElemMul(fr / pdf, tracer->Trace(reflected, record.Depth + 1).second * (record.Normal * wi));
}

void Reflective::PhotonMapping(const HitRecord& record)
{
    auto rf = reflectiveBRDF.GetCr(record);
    auto pr = std::max(rf.x, std::max(rf.y, rf.z));
    Random rand;
    if (rand.nextDouble() < pr)
    {
        Vec3D wo = -record.Ray.Direction;
        Vec3D wi;
        FP_TYPE pdf;
        RGBColor fr = reflectiveBRDF.sampleF(record, wi, wo, pdf);
        Ray reflected(record.HitPoint, wi, ElemMul(fr, record.Ray.Power));
        tracer->Trace(reflected, record.Depth + 1);
    }
}

RGBColor GlossyReflector::Shade(const HitRecord& record)
{
    RGBColor L(Matte::Shade(record));
    Vec3D wo = -record.Ray.Direction;

    auto samples = glossyBRDF.GetWiAndF(record, wo);
    RGBColor tmpL;
    for (auto& sample : samples)
    {
        auto wi = sample.first;
        auto frPDF = sample.second;
        Ray reflected(record.HitPoint, wi);
        tmpL += ElemMul(frPDF, tracer->Trace(reflected, record.Depth + 1).second * (record.Normal * wi));
    }
    L += tmpL / samples.size();
    return L;
}

std::pair<bool, RGBColor> GlossyReflector::PathShade(const HitRecord& record)
{
    auto d = diffuseBRDF.GetCd(record);
    auto s = glossyBRDF.GetCs(record);
    auto pd = (d.x + d.y + d.z) / (d.x + d.y + d.z + s.x + s.y + s.z);

    Vec3D wo = -record.Ray.Direction;
    Vec3D wi;
    FP_TYPE pdf;
    Random rand;
    auto randnum = rand.nextDouble();
    if (randnum < pd)
    {
        RGBColor fr = glossyBRDF.sampleF(record, wi, wo, pdf);
        Ray reflected(record.HitPoint, wi);
        auto traceResult = tracer->Trace(reflected, record.Depth + 1);
        return std::make_pair(traceResult.first, ElemMul(fr / pdf, traceResult.second * (record.Normal * wi)));
    }
    else
    {
        RGBColor f = diffuseBRDF.sampleF(record, wi, wo, pdf);
        Ray reflected(record.HitPoint, wi);
        auto traceResult = tracer->Trace(reflected, record.Depth + 1);
        return std::make_pair(traceResult.first, ElemMul(f / pdf, traceResult.second * (record.Normal * wi)));
    }
}

RGBColor GlossyReflector::PhotonShade(const HitRecord& record)
{
    float irrad[3], pos[3], normal[3];
    pos[0] = record.HitPoint.x; pos[1] = record.HitPoint.y; pos[2] = record.HitPoint.z;
    normal[0] = record.Normal.x; normal[1] = record.Normal.y; normal[2] = record.Normal.z;
    autoIrradianceEstimate(balancedPhotonMap, irrad, pos, normal, N_PHOTON);
    return RGBColor(irrad[0], irrad[1], irrad[2]);
}

void GlossyReflector::PhotonMapping(const HitRecord& record)
{
    auto d = diffuseBRDF.GetCd(record);
    auto s = glossyBRDF.GetCs(record);
    auto pr = std::max(d.x + s.x, std::max(d.y + s.y, d.z + s.z));
    auto pd = (d.x + d.y + d.z) / (d.x + d.y + d.z + s.x + s.y + s.z) * pr;
    auto ps = (s.x + s.y + s.z) / (d.x + d.y + d.z + s.x + s.y + s.z) * pr;

    Vec3D wi, wo = -record.Ray.Direction;
    FP_TYPE pdf;
    RGBColor f = diffuseBRDF.sampleF(record, wi, wo, pdf);
    Ray reflected(record.HitPoint, wi, ElemMul(f, record.Ray.Power / pd));

    float power[3]{ (float)(f.x * record.Ray.Power.x), (float)(f.y * record.Ray.Power.y), (float)(f.z * record.Ray.Power.z) };
    float pos[3]{ (float)record.HitPoint.x, (float)record.HitPoint.y, (float)record.HitPoint.z };
    float dir[3]{ (float)record.Ray.Direction.x, (float)record.Ray.Direction.y, (float)record.Ray.Direction.z };

    Random rand;
    auto randnum = rand.nextDouble();
    if (randnum < pd)
    {
        tracer->Trace(reflected, record.Depth + 1);
        photonMapLock.enter();
        storePhoton(photonMap, power, pos, dir);
        photonMapLock.exit();
    }
    else if (randnum < ps + pd)
    {
        RGBColor fr = glossyBRDF.sampleF(record, wi, wo, pdf);
        reflected = Ray(record.HitPoint, wi, ElemMul(fr, record.Ray.Power / ps));
        tracer->Trace(reflected, record.Depth + 1);
    }
    else
    {
        photonMapLock.enter();
        storePhoton(photonMap, power, pos, dir);
        photonMapLock.exit();
    }
}

RGBColor Transparent::Shade(const HitRecord& record)
{
    RGBColor L(Matte::Shade(record));
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

RGBColor Transparent::PhotonShade(const HitRecord& record)
{
    RGBColor L;
    Vec3D wo = -record.Ray.Direction;
    Vec3D wi;
    FP_TYPE pdf;
    RGBColor fr = reflectiveBRDF.sampleF(record, wi, wo, pdf);
    Ray reflected(record.HitPoint, wi);

    if (specularBTDF.tir(record))
    {
        L += tracer->Trace(reflected, record.Depth + 1).second * (record.Normal * wi);
    }
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

void Transparent::PhotonMapping(const HitRecord& record)
{
    Vec3D wo = -record.Ray.Direction;
    Vec3D wi;
    FP_TYPE pdf;
    RGBColor fr = reflectiveBRDF.sampleF(record, wi, wo, pdf);

    Random rand;
    if (specularBTDF.tir(record))
    {
        auto r = reflectiveBRDF.GetCr(record);
        auto pr = std::max(r.x, std::max(r.y, r.z));
        Ray reflected(record.HitPoint, wi, ElemMul(fr, record.Ray.Power / pr));
        tracer->Trace(reflected, record.Depth + 1);
    }
    else
    {
        auto r = reflectiveBRDF.GetCr(record);
        auto t = specularBTDF.GetCt(record);
        auto ptotal = std::max(r.x + t.x, std::max(r.y + t.y, r.z + t.z));
        auto pr = (r.x + r.y + r.z) / (r.x + r.y + r.z + t.x + t.y + t.z) * ptotal;
        auto pt = (t.x + t.y + t.z) / (r.x + r.y + r.z + t.x + t.y + t.z) * ptotal;
        auto randnum = rand.nextDouble();
        if (randnum < pr)
        {
            Vec3D wi, wo = -record.Ray.Direction;
            RGBColor f = reflectiveBRDF.sampleF(record, wi, wo, pdf);
            Ray reflected(record.HitPoint, wi, ElemMul(fr, record.Ray.Power / pr));
            tracer->Trace(reflected, record.Depth + 1);
        }
        else if (randnum < pt + pr)
        {
            Vec3D wt;
            RGBColor ft = specularBTDF.sampleF(record, wo, wt);
            Ray transmitted(record.HitPoint, wt, ElemMul(ft, record.Ray.Power / pt));
            tracer->Trace(transmitted, record.Depth + 1);
        }
    }

}
