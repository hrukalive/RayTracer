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
	virtual ~GeometricObject() {}
    virtual void SetMaterial(std::shared_ptr<Material> newMaterialPtr) { materialPtr = newMaterialPtr; }
	BBox& GetBoundingBox() { return boundingBox; }
};
