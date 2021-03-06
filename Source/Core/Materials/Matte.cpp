#include "Material.h"
#include "../World.h"
#include "../Tracer/Tracer.h"
#include "../Globals.h"

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
    autoIrradianceEstimate(balancedPhotonMap, irrad, pos, normal, viewPlane->NumNPhoton);
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

    storePhoton(photonMap, f, record);
}
