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
	void UpdateBoundingBox() override
	{
		boundingBox = BBox(a, a);
		boundingBox.Merge(BBox(b, b));
		boundingBox.Merge(BBox(c, c));
	}
public:
	MeshTriangle(Vec3D vertexA, Vec3D vertexB, Vec3D vertexC,
				 Vec3D normalA, Vec3D normalB, Vec3D normalC)
		: a(vertexA), b(vertexB), c(vertexC), na(normalA), nb(normalB), nc(normalC)
	{
		nf = (b - a) ^ (c - a);
		UpdateBoundingBox();
	}
	virtual ~MeshTriangle() {}
	void SetSmooth(bool isSmooth) { this->isSmooth = isSmooth; }
	Vec3D getA() { return a; }
	Vec3D getB() { return b; }
	Vec3D getC() { return c; }
	Vec3D getNormal() { return nf.normalised(); }

	virtual HitRecord Hit(const Ray& ray) override
	{
		HitRecord record;
		auto temp = ((a - ray.Origin) * nf) / (ray.Direction * nf);
		if (temp > kEpsilon)
		{
			auto p = ray.GetPoint(temp);
			auto denom = nf * nf;
			auto l1 = (nf * ((b - a) ^ (p - a))) / denom;
			auto l2 = (nf * ((c - b) ^ (p - b))) / denom;
			auto l3 = 1.0 - l1 - l2;
			if (l1 >= 0.0 && l1 <= 1.0 && l2 >= 0.0 && l2 <= 1.0 && l3 >= 0.0 && l3 <= 1.0)
			{
				record.Hit = true;
				if (!isSmooth)
				{
					record.Normal = (nf * -ray.Direction > 0.0 ? nf.normalised() : -nf.normalised());
				}
				else
				{
					auto tmpn = nc * l1 + na * l2 + nb * l3;
					record.Normal = (tmpn * -ray.Direction > 0.0 ? tmpn.normalised() : -tmpn.normalised());
				}
				record.HitPoint = p;
				record.MaterialPtr = materialPtr;
				record.Ray = ray;
				record.T = temp;
			}
		}
		return record;
	}
};

class Mesh : public RayTracer::Grid
{
public:
	~Mesh() {}
	void SetMaterial(std::shared_ptr<Material>& material) override
	{
		this->materialPtr = material;
	}
	virtual HitRecord Hit(const Ray& ray) override
	{
		HitRecord record = RayTracer::Grid::Hit(ray);
		record.MaterialPtr = materialPtr;
		return record;
	}
};
