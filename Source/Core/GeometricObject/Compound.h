/*
  ==============================================================================

    Compound.h
    Created: 4 Oct 2018 4:04:19pm
    Author:  Bowen Sun

  ==============================================================================
*/

#pragma once
#include "GeometricObject.h"

class Compound : public GeometricObject
{
protected:
	std::vector<std::shared_ptr<GeometricObject>> objects;
    void UpdateBoundingBox() override;
public:
    virtual ~Compound() = default;
    virtual void AddObject(std::shared_ptr<GeometricObject> obj);
    void SetMaterial(std::shared_ptr<Material> newMaterialPtr) override;

    HitRecord Hit(const Ray& ray) override;
};
