#pragma once
#include "../Utility.h"
#include "../Sampler.h"

class BRDF
{
protected:
	std::unique_ptr<Sampler> SamplerPtr;
public:
    virtual ~BRDF() = default;
	virtual RGBColor f(const HitRecord& record, const Vec3D& wi, const Vec3D& wo) const = 0;
	virtual RGBColor sampleF(const HitRecord& record, const Vec3D& wi, const Vec3D& wo, const FP_TYPE& pdf) const = 0;
	virtual RGBColor rho(const HitRecord& record, const Vec3D& wo) const = 0;
};

class Lambertian : public BRDF
{
    FP_TYPE kd;
	RGBColor cd;
public:
    Lambertian();
    Lambertian(const FP_TYPE kd, const RGBColor& cd);
    virtual ~Lambertian() = default;

    void SetKd(const FP_TYPE newKd);
    void SetCd(const RGBColor& newCd);

    virtual RGBColor f(const HitRecord& record, const Vec3D& wi, const Vec3D& wo) const override;
    virtual RGBColor sampleF(const HitRecord& record, const Vec3D& wi, const Vec3D& wo, const FP_TYPE& pdf) const override;
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

    virtual RGBColor f(const HitRecord& record, const Vec3D& wi, const Vec3D& wo) const override;
    virtual RGBColor sampleF(const HitRecord& record, const Vec3D& wi, const Vec3D& wo, const FP_TYPE& pdf) const override;
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

    virtual RGBColor f(const HitRecord& record, const Vec3D& wi, const Vec3D& wo) const override;
    virtual RGBColor sampleF(const HitRecord& record, const Vec3D& wi, const Vec3D& wo, const FP_TYPE& pdf) const override;
    virtual RGBColor rho(const HitRecord& record, const Vec3D& wo) const override;
};
