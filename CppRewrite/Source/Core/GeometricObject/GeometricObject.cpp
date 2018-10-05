/*
  ==============================================================================

    GeometricObject.cpp
    Created: 5 Oct 2018 2:23:49pm
    Author:  Bowen Sun

  ==============================================================================
*/

#include "GeometricObject.h"
#include "BBox.h"

GeometricObject::GeometricObject(const GeometricObject& other)
{
    this->materialPtr = other.materialPtr;
    this->boundingBox = new BBox(*other.boundingBox);
}

GeometricObject::~GeometricObject()
{
    delete this->boundingBox;
}
