/*
  ==============================================================================

    Rectangle.h
    Created: 28 Sep 2018 11:00:58am
    Author:  Bowen Sun

  ==============================================================================
*/

#pragma once
#include "GeometricObject.h"

namespace RayTracer
{
    class Rectangle : public GeometricObject
    {
        Point3D a;
        Vec3D u, v, n;
    protected:
        void UpdateBoundingBox() override;
    public:
        Rectangle(Point3D origin, Vec3D uDirection, Vec3D vDirection);
        virtual ~Rectangle() = default;

        void setOrigin(Point3D origin);
        void setU(Vec3D uDirection);
        void setV(Vec3D vDirection);
        Point3D getOrigin();
        Vec3D getU();
        Vec3D getV();

        std::vector<std::pair<Point3D, Vec3D>> Sample(int numSamples = -1) override;
        std::pair<Point3D, Vec3D> SampleSingle() override;
        FP_TYPE pdf(const HitRecord& record) override;
        Vec3D GetNormal(const Point3D p);

        virtual HitRecord Hit(const Ray& ray) override;
    };
}
