/*
  ==============================================================================

    Plane.h
    Created: 28 Sep 2018 11:00:58am
    Author:  Bowen Sun

  ==============================================================================
*/

#pragma once
#include "GeometricObject.h"

class Plane : public GeometricObject
{
	Point3D a;
	Vec3D u, v, n;
protected:
    void UpdateBoundingBox() override;
public:
    Plane(Point3D origin, Vec3D uDirection, Vec3D vDirection);
    virtual ~Plane() = default;

    void setOrigin(Point3D origin);
    void setU(Vec3D uDirection);
    void setV(Vec3D vDirection);
    Point3D getOrigin();
    Vec3D getU();
    Vec3D getV();

    virtual Point3D Sample() override;
    virtual Vec3D GetNormal(const Point3D p) override;
    virtual FP_TYPE pdf(const HitRecord& record) override;

    virtual HitRecord Hit(const Ray& ray) override;
};
