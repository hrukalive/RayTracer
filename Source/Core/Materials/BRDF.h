#pragma once
#include "../Utility.h"
#include "../Sampler.h"

class BRDF
{
protected:
	std::unique_ptr<Sampler> SamplerPtr;
public:
    virtual ~BRDF() {}
	virtual RGBColor f(const HitRecord& record, const Vec3D& wi, const Vec3D& wo) const = 0;
	virtual RGBColor sampleF(const HitRecord& record, const Vec3D& wi, const Vec3D& wo) const = 0;
	virtual RGBColor rho(const HitRecord& record, const Vec3D& wo) const = 0;
};

class Lambertian : public BRDF
{
    FP_TYPE kd;
	RGBColor cd;
public:
    Lambertian() : kd(0.0), cd(BLACK) {}
    Lambertian(const FP_TYPE kd, const RGBColor& cd)
        : kd(kd), cd(cd) {}
    virtual ~Lambertian() {}
    void SetKd(const FP_TYPE newKd) { kd = newKd; }
    void SetCd(const RGBColor& newCd) { cd = newCd; }

	virtual RGBColor f(const HitRecord& record, const Vec3D& wi, const Vec3D& wo) const
	{
		return (cd * kd) * INV_PI;
	}
	virtual RGBColor sampleF(const HitRecord& record, const Vec3D& wi, const Vec3D& wo) const
	{
		return BLACK;
	}
	virtual RGBColor rho(const HitRecord& record, const Vec3D& wo) const
	{
		return cd * kd;
	}
};

class PerfectSpecular : public BRDF
{
    FP_TYPE kd;
    RGBColor cd;
public:
    PerfectSpecular() : kd(0.0), cd(BLACK) {}
    PerfectSpecular(const FP_TYPE kd, const RGBColor& cd)
        : kd(kd), cd(cd) {}
    virtual ~PerfectSpecular() {}
    void SetKd(const FP_TYPE newKd) { kd = newKd; }
    void SetCd(const RGBColor& newCd) { cd = newCd; }
    
    virtual RGBColor f(const HitRecord& record, const Vec3D& wi, const Vec3D& wo) const
    {
        return BLACK;
    }
    virtual RGBColor sampleF(const HitRecord& record, const Vec3D& wi, const Vec3D& wo) const
    {
        return BLACK;
    }
    virtual RGBColor rho(const HitRecord& record, const Vec3D& wo) const
    {
        return BLACK;
    }
};

class GlossySpecular : public BRDF
{
    FP_TYPE ks, exp;
    RGBColor cs;
public:
    GlossySpecular() : ks(1.0), exp(10.0), cs(WHITE) {}
    GlossySpecular(const FP_TYPE ks, const FP_TYPE exp, const RGBColor& cs)
        : ks(ks), exp(exp), cs(cs) {}
    virtual ~GlossySpecular() {}
    void SetKs(const FP_TYPE newKs) { ks = newKs; }
    void SetCs(const RGBColor& newCs) { cs = newCs; }
    void SetE(const FP_TYPE newExp) { exp = newExp; }
    
    virtual RGBColor f(const HitRecord& record, const Vec3D& wi, const Vec3D& wo) const
    {
        RGBColor L;
        auto ndotwi = record.Normal * wi;
        auto r = -wi + (record.Normal * 2.0 * ndotwi);
        auto rdotwo = r * wo;
        if (rdotwo > 0.0)
        {
            L = cs * (ks * pow(rdotwo, exp));
        }
        return L;
    } // From book chapter 15
    
    virtual RGBColor sampleF(const HitRecord& record, const Vec3D& wi, const Vec3D& wo) const
    {
        return BLACK;
    }
    virtual RGBColor rho(const HitRecord& record, const Vec3D& wo) const
    {
        return BLACK;
    }
};
