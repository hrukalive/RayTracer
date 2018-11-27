#include "Material.h"
#include "../Tracer/Tracer.h"
#include "../Globals.h"

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
            Vec3D wi;
            RGBColor f = reflectiveBRDF.sampleF(record, wi, wo, pdf);
            Ray reflected(record.HitPoint, wi, ElemMul(f, record.Ray.Power / pr));
            tracer->Trace(reflected, record.Depth + 1);
        }
        else if (randnum < pr + pt)
        {
            Vec3D wt;
            RGBColor ft = specularBTDF.sampleF(record, wo, wt);
            Ray transmitted(record.HitPoint, wt, ElemMul(ft, record.Ray.Power / pt));
            tracer->Trace(transmitted, record.Depth + 1);
        }
    }
}
