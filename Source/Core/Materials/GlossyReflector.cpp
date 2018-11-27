#include "Material.h"
#include "../Tracer/Tracer.h"
#include "../Globals.h"

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
        storePhoton(photonMap, power, pos, dir);
    }
    else if (randnum < ps + pd)
    {
        RGBColor fr = glossyBRDF.sampleF(record, wi, wo, pdf);
        reflected = Ray(record.HitPoint, wi, ElemMul(fr, record.Ray.Power / ps));
        tracer->Trace(reflected, record.Depth + 1);
    }
    else
    {
        storePhoton(photonMap, power, pos, dir);
    }
}
