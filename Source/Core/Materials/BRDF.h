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
	float kd;
	RGBColor cd;
public:
    Lambertian() : kd(0.0), cd(BLACK) {}
    Lambertian(const float kd, const RGBColor& cd)
        : kd(kd), cd(cd) {}
    virtual ~Lambertian() {}
    void SetKd(const float kd) { this->kd = kd; }
    void SetCd(const RGBColor& cd) { this->cd = cd; }

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
    float kd;
    RGBColor cd;
public:
    PerfectSpecular() : kd(0.0), cd(BLACK) {}
    PerfectSpecular(const float kd, const RGBColor& cd)
        : kd(kd), cd(cd) {}
    virtual ~PerfectSpecular() {}
    void SetKd(const float kd) { this->kd = kd; }
    void SetCd(const RGBColor& cd) { this->cd = cd; }
    
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
    float ks, exp;
    RGBColor cs;
public:
    GlossySpecular() : ks(1.0), exp(10.0), cs(WHITE) {}
    GlossySpecular(const float ks, const float exp, const RGBColor& cs)
        : ks(ks), exp(exp), cs(cs) {}
    virtual ~GlossySpecular() {}
    void SetKs(const float ks) { this->ks = ks; }
    void SetCs(const RGBColor& cs) { this->cs = cs; }
    void SetE(const float exp) { this->exp = exp; }
    
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
