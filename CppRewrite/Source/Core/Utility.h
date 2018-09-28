/*
  ==============================================================================

    Utility.h
    Created: 28 Sep 2018 11:07:40am
    Author:  Bowen Sun

  ==============================================================================
*/

#pragma once

#include "../../JuceLibraryCode/JuceHeader.h"

#define FP_TYPE double
#define kEpsilon 0.001

struct Ray
{
    Vector3D<FP_TYPE> Origin;
    Vector3D<FP_TYPE> Direction;
    Ray(Vector3D<FP_TYPE> origin, Vector3D<FP_TYPE> direction) : Origin(origin), Direction(direction) {}
	Vector3D<FP_TYPE> GetPoint(FP_TYPE t)
	{
		return Origin + (Direction * t);
	}
};

struct HitRecord
{
	Vector3D<FP_TYPE> Normal;
	Vector3D<FP_TYPE> Position;
	FP_TYPE T = 0.0f;
	HitRecord(Vector3D<FP_TYPE> normal, Vector3D<FP_TYPE> position, FP_TYPE t) 
		: Normal(normal), Position(position), T(t) {}
};