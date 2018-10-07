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
	virtual ~GeometricObject() {}
    virtual void SetMaterial(std::shared_ptr<Material>& materialPtr) { this->materialPtr = materialPtr; }
	BBox& GetBoundingBox() { return boundingBox; }
};

#include "Sphere.h"
#include "Plane.h"
#include "Triangle.h"
#include "Instance.h"
#include "Compound.h"
#include "Grid.h"
#include "Mesh.h"
