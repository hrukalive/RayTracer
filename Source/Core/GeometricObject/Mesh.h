/*
  ==============================================================================

    Mesh.h
    Created: 4 Oct 2018 4:09:11pm
    Author:  Bowen Sun

  ==============================================================================
*/

#pragma once
#include "Grid.h"

class MeshTriangle : public GeometricObject
{
	Vec3D a, b, c, na, nb, nc, nf;
	bool isSmooth = true;
protected:
    void UpdateBoundingBox() override;
public:
    MeshTriangle(Vec3D vertexA, Vec3D vertexB, Vec3D vertexC, Vec3D normalA, Vec3D normalB, Vec3D normalC);
    virtual ~MeshTriangle();
    void SetSmooth(bool newIsSmooth);
    Vec3D getA();
    Vec3D getB();
    Vec3D getC();
    Vec3D getNormal();

    virtual HitRecord Hit(const Ray& ray) override;
};

class Mesh : public RayTracer::Grid
{
public:
    virtual ~Mesh();
    void SetMaterial(std::shared_ptr<Material> newMaterialPtr) override;
    virtual HitRecord Hit(const Ray& ray) override;
};
