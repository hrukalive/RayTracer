#pragma once
#include "../Utility.h"
#include "../Sampler.h"
#include "../World.h"

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
    virtual ~Lambertian() {}
	void SetKd(const float kd) { this->kd = kd; }
	void SetCd(const RGBColor& cd) { this->cd = cd; }

	virtual RGBColor f(const HitRecord& record, const Vec3D& wi, const Vec3D& wo) const
	{
		return (cd * kd) * INV_PI;
	}
	virtual RGBColor sampleF(const HitRecord& record, const Vec3D& wi, const Vec3D& wo) const
	{
		return Vec3D(0.0, 0.0, 0.0);
	}
	virtual RGBColor rho(const HitRecord& record, const Vec3D& wo) const
	{
		return cd * kd;
	}
};
