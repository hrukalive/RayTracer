/*
  ==============================================================================

    Material.h
    Created: 28 Sep 2018 11:03:39am
    Author:  Bowen Sun

  ==============================================================================
*/

#pragma once
#include "../Utility/Utility.h"
#include "Base/BRDF.h"
#include "Base/BTDF.h"

class Material
{
public:
	virtual ~Material() = default;
    virtual RGBColor GetLe(const HitRecord& record);
    virtual RGBColor Shade(const HitRecord& record);
    virtual std::pair<bool, RGBColor> PathShade(const HitRecord& record);
    virtual RGBColor PhotonShade(const HitRecord &record);
    virtual void PhotonMapping(const HitRecord& record) {}
};

class ConstColor : public Material
{
	RGBColor c = RED;
public:
    ConstColor() = default;
    ConstColor(RGBColor color);
    virtual ~ConstColor() = default;
    RGBColor Shade(const HitRecord& record) override;
    std::pair<bool, RGBColor> PathShade(const HitRecord& record) override;
    RGBColor PhotonShade(const HitRecord &record) override;
};

class NormalShade : public Material
{
public:
	virtual ~NormalShade() = default;
    RGBColor Shade(const HitRecord& record) override;
    std::pair<bool, RGBColor> PathShade(const HitRecord& record) override;
    RGBColor PhotonShade(const HitRecord &record) override;
};

class Matte : public Material
{
protected:
	Lambertian ambientBRDF, diffuseBRDF;
public:
    virtual ~Matte() = default;
    void SetKa(const FP_TYPE ka);
    void SetKd(const FP_TYPE kd);
    void SetCd(const RGBColor& c);
    void SetCd(const std::shared_ptr<Texture> c);
	RGBColor Shade(const HitRecord& record) override;
    std::pair<bool, RGBColor> PathShade(const HitRecord& record) override;
    RGBColor PhotonShade(const HitRecord &record) override;
    void PhotonMapping(const HitRecord& record) override;
};

class Phong : public Material
{
    Lambertian ambientBRDF, diffuseBRDF;
    GlossySpecular specularBRDF;
public:
    virtual ~Phong() = default;
    void SetKa(const FP_TYPE ka);
    void SetKd(const FP_TYPE kd);
    void SetKs(const FP_TYPE ks);
    void SetCd(const RGBColor& c);
    void SetCd(const std::shared_ptr<Texture> c);
    void SetCs(const RGBColor& c);
    void SetE(const FP_TYPE exp);
	RGBColor Shade(const HitRecord& record) override;
    std::pair<bool, RGBColor> PathShade(const HitRecord& record) override;
    RGBColor PhotonShade(const HitRecord &record) override;
    void PhotonMapping(const HitRecord& record) override;
};

class Emissive : public Material
{
    FP_TYPE ls;
    RGBColor ce;
public:
    virtual ~Emissive() = default;
    void SetLs(const FP_TYPE newLs)
    {
        ls = newLs;
    }
    void SetCe(const RGBColor& newCe)
    {
        ce = newCe;
    }
    virtual RGBColor GetLe(const HitRecord& record) override
    {
        return ce * ls;
    }
    RGBColor Shade(const HitRecord& record) override;
    std::pair<bool, RGBColor> PathShade(const HitRecord& record) override;
    RGBColor PhotonShade(const HitRecord &record) override;
};

class Reflective : public Matte
{
    PerfectSpecular reflectiveBRDF;
public:
    virtual ~Reflective() = default;
    void SetKr(const FP_TYPE newKr)
    {
        reflectiveBRDF.SetKr(newKr);
    }
    void SetCr(const RGBColor& newCr)
    {
        reflectiveBRDF.SetCr(newCr);
    }
    RGBColor Shade(const HitRecord& record) override;
    std::pair<bool, RGBColor> PathShade(const HitRecord& record) override;
    RGBColor PhotonShade(const HitRecord &record) override;
    void PhotonMapping(const HitRecord& record) override;
};

class GlossyReflector : public Matte
{
    GlossySpecular glossyBRDF;
public:
    virtual ~GlossyReflector() = default;
    void SetKs(const FP_TYPE kr)
    {
        glossyBRDF.SetKs(kr);
    }
    void SetCs(const RGBColor& c)
    {
        glossyBRDF.SetCs(c);
    }
    void SetE(const FP_TYPE exp)
    {
        glossyBRDF.SetE(exp);
    }
    RGBColor Shade(const HitRecord& record) override;
    std::pair<bool, RGBColor> PathShade(const HitRecord& record) override;
    RGBColor PhotonShade(const HitRecord &record) override;
    void PhotonMapping(const HitRecord& record) override;
};

class Transparent : public Matte
{
    PerfectSpecular reflectiveBRDF;
    PerfectTransmitter specularBTDF;
public:
    virtual ~Transparent() = default;
    void SetKr(const FP_TYPE newKr)
    {
        reflectiveBRDF.SetKr(newKr);
    }
    void SetCr(const RGBColor& newCr)
    {
        reflectiveBRDF.SetCr(newCr);
    }
    void SetCt(const RGBColor& newCt)
    {
        specularBTDF.SetCt(newCt);
    }
    void SetKt(FP_TYPE newKt) { specularBTDF.SetKt(newKt); }
    void SetIOR(FP_TYPE newIOR) { specularBTDF.SetIOR(newIOR); }
    RGBColor Shade(const HitRecord& record) override;
    std::pair<bool, RGBColor> PathShade(const HitRecord& record) override;
    RGBColor PhotonShade(const HitRecord &record) override;
    void PhotonMapping(const HitRecord& record) override;
};
