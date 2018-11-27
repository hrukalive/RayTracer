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

class GeometricObject : public IObject
{
protected:
    std::shared_ptr<Material> materialPtr;
    BBox boundingBox;
	virtual void UpdateBoundingBox() = 0;
public:
    virtual ~GeometricObject() = default;
    virtual void SetMaterial(std::shared_ptr<Material> newMaterialPtr);
    BBox& GetBoundingBox();
    virtual std::vector<std::pair<Point3D, Vec3D>> Sample(int numSamples = -1);
    virtual std::pair<Point3D, Vec3D> SampleSingle();
    virtual FP_TYPE pdf(const HitRecord& record);

    virtual std::vector<HitRecord> getHitInterval(const Ray& ray);
};

#include "Basic/Box.h"
#include "Basic/Rectangle.h"
#include "Basic/Disk.h"
#include "Basic/Triangle.h"
#include "Basic/OpenCylinder.h"
#include "Basic/Sphere.h"
#include "Basic/Torus.h"
#include "Basic/CSG.h"

