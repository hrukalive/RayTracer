#include "Material.h"
#include "../Tracer/Tracer.h"
#include "../Globals.h"

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
