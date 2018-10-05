/*
  ==============================================================================

    Compound.h
    Created: 4 Oct 2018 4:04:19pm
    Author:  Bowen Sun

  ==============================================================================
*/

#pragma once
#include "GeometricObject.h"
#include "BBox.h"

class Compound : public GeometricObject
{
protected:
	BBox boundingBox;
};
