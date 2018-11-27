/*
  ==============================================================================

    BTDF.h
    Created: 6 Nov 2018 1:24:17pm
    Author:  bowen

  ==============================================================================
*/

#pragma once
#include "../Utility/Utility.h"

class BTDF
{
public:
    virtual ~BTDF() = default;
    virtual RGBColor f(const HitRecord& record, const Vec3D& wo, Vec3D& wt) const = 0;
    virtual RGBColor sampleF(const HitRecord& record, const Vec3D& wo, Vec3D& wt) const = 0;
    virtual RGBColor rho(const HitRecord& record, const Vec3D& wo) const = 0;
    virtual bool tir(const HitRecord& record) const = 0;
};

class PerfectTransmitter : public BTDF
{
    RGBColor ct;
    FP_TYPE kt, ior;
public:
    virtual ~PerfectTransmitter() = default;
    void SetCt(RGBColor newCt) { ct = newCt; }
    void SetKt(FP_TYPE newKt) { kt = newKt; }
    void SetIOR(FP_TYPE newIOR) { ior = newIOR; }
    RGBColor GetCt(const HitRecord& record) { return ct * kt; }
    RGBColor f(const HitRecord& record, const Vec3D& wo, Vec3D& wt) const override;
    RGBColor sampleF(const HitRecord& record, const Vec3D& wo, Vec3D& wt) const override;
    RGBColor rho(const HitRecord& record, const Vec3D& wo) const override;
    bool tir(const HitRecord& record) const override;
};
