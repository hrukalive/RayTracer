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
    virtual std::vector<std::pair<Point3D, Vec3D>> Sample()
    {
        return std::vector<std::pair<Point3D, Vec3D>>();
    }
    //virtual Vec3D GetNormal(const Point3D p) = 0;
    //virtual FP_TYPE pdf(const HitRecord& record) = 0;
};
