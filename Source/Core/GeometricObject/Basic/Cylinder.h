/*
  ==============================================================================

    Cylinder.h
    Created: 17 Nov 2018 9:42:58pm
    Author:  bowen

  ==============================================================================
*/

#pragma once

#include "../Compound.h"

class Cylinder : public Compound
{
    FP_TYPE h, r;
    Point3D bc;
protected:
    void UpdateBoundingBox() override;
public:
    Cylinder(Point3D bottomCenter, FP_TYPE radius, FP_TYPE height);
    virtual ~Cylinder() = default;
};
