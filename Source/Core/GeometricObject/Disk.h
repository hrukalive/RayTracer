/*
  ==============================================================================

    Disk.h
    Created: 17 Nov 2018 9:42:51pm
    Author:  bowen

  ==============================================================================
*/

#pragma once
#include "GeometricObject.h"

class Disk : public GeometricObject
{
    Point3D center;
    FP_TYPE r;
    Vec3D n;
protected:
    void UpdateBoundingBox() override;
public:
    Disk(Point3D center, FP_TYPE radius, Vec3D normal);
    virtual ~Disk() = default;

    //std::vector<std::pair<Point3D, Vec3D>> Sample() override;
    //std::pair<Point3D, Vec3D> SampleSingle() override;
    //FP_TYPE pdf(const HitRecord& record) override;

    virtual HitRecord Hit(const Ray& ray) override;
};
