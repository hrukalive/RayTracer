/*
  ==============================================================================

    BTDF.cpp
    Created: 6 Nov 2018 1:24:17pm
    Author:  bowen

  ==============================================================================
*/

#include "BTDF.h"


RGBColor PerfectTransmitter::f(const HitRecord& record, const Vec3D& wo, Vec3D& wt) const
{
    return BLACK;
}
RGBColor PerfectTransmitter::sampleF(const HitRecord& record, const Vec3D& wo, Vec3D& wt) const
{
    Vec3D n = record.Normal;
    FP_TYPE cosTheta_i = record.Normal * wo;
    FP_TYPE eta = ior;

    if (cosTheta_i < 0.0)
    {
        cosTheta_i = -cosTheta_i;
        n = -n;
        eta = 1.0 / eta;
    }

    FP_TYPE cosTheta2 = sqrt(1.0 - (1.0 - cosTheta_i * cosTheta_i) / (eta * eta));
    wt = -wo / eta - n * (cosTheta2 - cosTheta_i / eta);
    return ct * (kt / (eta * eta)) / fabs(record.Normal * wt);
}
RGBColor PerfectTransmitter::rho(const HitRecord& record, const Vec3D& wo) const
{
    return BLACK;
}
bool PerfectTransmitter::tir(const HitRecord& record) const
{
    Vec3D wo = -record.Ray.Direction;
    FP_TYPE cosTheta_i = record.Normal * wo;
    FP_TYPE eta = ior;
    if (cosTheta_i < 0.0)
        eta = 1.0 / eta;
    return 1.0 - (1.0 - cosTheta_i * cosTheta_i) / (eta * eta) < 0.0;
}
