#pragma once
#include "../Utility/Utility.h"
#include "../Sampler/Sampler.h"
#include "Texture.h"

class BRDF
{
public:
    virtual ~BRDF() = default;
	virtual RGBColor f(const HitRecord& record, const Vec3D& wi, const Vec3D& wo) const = 0;
	virtual RGBColor sampleF(const HitRecord& record, Vec3D& wi, const Vec3D& wo, FP_TYPE& pdf) const = 0;
	virtual RGBColor rho(const HitRecord& record, const Vec3D& wo) const = 0;
};

class Lambertian : public BRDF
{
    FP_TYPE kd;
    std::shared_ptr<Texture> cd;
public:
    Lambertian();
    virtual ~Lambertian() = default;

    void SetKd(const FP_TYPE newKd);
    void SetCd(const RGBColor& newCd);
    void SetCd(const std::shared_ptr<Texture> newCd);
    RGBColor GetCd(const HitRecord& record) { return cd->getColor(record) * kd; }

    virtual RGBColor f(const HitRecord& record, const Vec3D& wi, const Vec3D& wo) const override;
    virtual RGBColor sampleF(const HitRecord& record, Vec3D& wi, const Vec3D& wo, FP_TYPE& pdf) const override;
    virtual RGBColor rho(const HitRecord& record, const Vec3D& wo) const override;
};

class PerfectSpecular : public BRDF
{
    FP_TYPE kr;
    RGBColor cr;
public:
    PerfectSpecular();
    PerfectSpecular(const FP_TYPE kr, const RGBColor& cr);
    virtual ~PerfectSpecular() = default;

    void SetKr(const FP_TYPE newKr);
    void SetCr(const RGBColor& newCr);
    RGBColor GetCr(const HitRecord& record) { return cr * kr; }

    virtual RGBColor f(const HitRecord& record, const Vec3D& wi, const Vec3D& wo) const override;
    virtual RGBColor sampleF(const HitRecord& record, Vec3D& wi, const Vec3D& wo, FP_TYPE& pdf) const override;
    virtual RGBColor rho(const HitRecord& record, const Vec3D& wo) const override;
};

class GlossySpecular : public BRDF
{
    FP_TYPE ks, exp;
    RGBColor cs;
public:
    GlossySpecular();
    GlossySpecular(const FP_TYPE ks, const FP_TYPE exp, const RGBColor& cs);
    virtual ~GlossySpecular() = default;

    void SetKs(const FP_TYPE newKs);
    void SetCs(const RGBColor& newCs);
    void SetE(const FP_TYPE newExp);
    RGBColor GetCs(const HitRecord& record) { return cs * ks; }

    virtual RGBColor f(const HitRecord& record, const Vec3D& wi, const Vec3D& wo) const override;
    virtual RGBColor sampleF(const HitRecord& record, Vec3D& wi, const Vec3D& wo, FP_TYPE& pdf) const override;
    virtual RGBColor rho(const HitRecord& record, const Vec3D& wo) const override;

    std::vector<std::pair<Vec3D, RGBColor>> GetWiAndF(const HitRecord& record, const Vec3D& wo);
};
