/*
  ==============================================================================

    GeometricObject.h
    Created: 28 Sep 2018 11:00:02am
    Author:  Bowen Sun

  ==============================================================================
*/

#pragma once

#include "IObject.h"
#include "BBox.h"
#include "../Utility.h"

class GeometricObject : public IObject
{
protected:
    std::shared_ptr<Material> materialPtr;
    BBox boundingBox;
	virtual void UpdateBoundingBox() = 0;
public:
    virtual ~GeometricObject() = default;
    virtual void SetMaterial(std::shared_ptr<Material> newMaterialPtr) { materialPtr = newMaterialPtr; }
	BBox& GetBoundingBox() { return boundingBox; }
    virtual std::vector<std::pair<Point3D, Vec3D>> Sample(int numSamples = -1)
    {
        return std::vector<std::pair<Point3D, Vec3D>>();
    }
    virtual std::pair<Point3D, Vec3D> SampleSingle()
    {
        return std::make_pair(ZERO, ZERO);
    }
    virtual FP_TYPE pdf(const HitRecord& record)
    {
        auto diff = boundingBox.GetMaxPoint() - boundingBox.GetMinPoint();
        return 1.0 / (2 * (diff.x * diff.y + diff.y * diff.z + diff.x * diff.z));
    }

    virtual std::vector<HitRecord> getHitInterval(const Ray& ray);
};
