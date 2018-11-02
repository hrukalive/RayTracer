/*
  ==============================================================================

    BBox.h
    Created: 4 Oct 2018 2:52:52pm
    Author:  Bowen Sun

  ==============================================================================
*/

#pragma once

#include "IObject.h"

class BBox : public IObject
{
	Point3D bboxMin, bboxMax;
	FP_TYPE diagLength = 0.0;
	FP_TYPE delta = (diagLength * kBBOXRatio > KBBOXMax ? KBBOXMax : (diagLength * kBBOXRatio));
public:
    BBox();
    BBox(const Point3D min, const Point3D max);
    virtual ~BBox() = default;
    void SetBoundingBox(const Point3D min, const Point3D max);
    void Merge(const BBox& other);
    Point3D GetMinPoint();
    Point3D GetMaxPoint();
    Vec3D GetNormal(const int face) const;
    bool Inside(const Point3D point);

    HitRecord Hit(const Ray& ray) override;
};
