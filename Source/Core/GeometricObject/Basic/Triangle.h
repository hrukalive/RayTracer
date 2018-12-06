/*
  ==============================================================================

    Triangle.h
    Created: 28 Sep 2018 11:03:10am
    Author:  Bowen Sun

  ==============================================================================
*/

#pragma once
#include "../GeometricObject.h"

class Triangle : public GeometricObject
{
	Vec3D a, b, c, n;
    void UpdateNormal();
protected:
    void UpdateBoundingBox() override;
public:
    Triangle(Vec3D vertexA, Vec3D vertexB, Vec3D vertexC);
    virtual ~Triangle() = default;
    void setA(Vec3D vertexA);
    void setB(Vec3D vertexB);
    void setC(Vec3D vertexC);
    Vec3D getA();
    Vec3D getB();
    Vec3D getC();
    Vec3D getNormal();

    virtual HitRecord Hit(const Ray& ray) override;

    static std::shared_ptr<GeometricObject> parse(StringArray& cmd, std::unordered_map<String, std::shared_ptr<void>>& env);
};
