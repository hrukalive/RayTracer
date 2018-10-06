/*
  ==============================================================================

    Instance.h
    Created: 4 Oct 2018 4:04:36pm
    Author:  Bowen Sun

  ==============================================================================
*/

#pragma once
#include "GeometricObject.h"

class Instance : public GeometricObject
{
	std::shared_ptr<GeometricObject> objectPtr;
	Matrix forwardMatrix = Matrix::identity(4);
	Matrix invMatrix = Matrix::identity(4);
	void Transform(const Matrix& transform, const Matrix& invTransform)
	{
		forwardMatrix = transform * forwardMatrix;
		invMatrix = invMatrix * invTransform;
		UpdateBoundingBox();
	}
protected:
	void UpdateBoundingBox()
	{
		if (!objectPtr)
			return;
		BBox& objBBox = objectPtr->GetBoundingBox();
		auto min = objBBox.GetMinPoint();
		auto max = objBBox.GetMaxPoint();
		auto x0 = min.x, y0 = min.y, z0 = min.z;
		auto x1 = max.x, y1 = max.y, z1 = max.z;
		auto tmp = MatrixMulPoint(forwardMatrix, Point3D(x0, y0, z0));
		boundingBox = BBox(tmp, tmp);
		tmp = MatrixMulPoint(forwardMatrix, Point3D(x1, y0, z0));
		boundingBox.Merge(BBox(tmp, tmp));
		tmp = MatrixMulPoint(forwardMatrix, Point3D(x0, y1, z0));
		boundingBox.Merge(BBox(tmp, tmp));
		tmp = MatrixMulPoint(forwardMatrix, Point3D(x0, y0, z1));
		boundingBox.Merge(BBox(tmp, tmp));
		tmp = MatrixMulPoint(forwardMatrix, Point3D(x1, y1, z0));
		boundingBox.Merge(BBox(tmp, tmp));
		tmp = MatrixMulPoint(forwardMatrix, Point3D(x0, y1, z1));
		boundingBox.Merge(BBox(tmp, tmp));
		tmp = MatrixMulPoint(forwardMatrix, Point3D(x1, y0, z1));
		boundingBox.Merge(BBox(tmp, tmp));
		tmp = MatrixMulPoint(forwardMatrix, Point3D(x1, y1, z1));
		boundingBox.Merge(BBox(tmp, tmp));
	}
public:
	Instance() {}
	Instance(std::shared_ptr<GeometricObject> objectPtr) : objectPtr(objectPtr)
	{
		UpdateBoundingBox();
	}
	virtual ~Instance() {}
	void SetObject(std::shared_ptr<GeometricObject> objectPtr)
	{
		this->objectPtr = objectPtr;
		UpdateBoundingBox();
	}
	void Translate(const FP_TYPE dx, const FP_TYPE dy, const FP_TYPE dz)
	{
		auto m = Matrix::identity(4);
		SetMatrix(m, 0, 3, dx);
		SetMatrix(m, 1, 3, dy);
		SetMatrix(m, 2, 3, dz);
		auto im = Matrix::identity(4);
		SetMatrix(im, 0, 3, -dx);
		SetMatrix(im, 1, 3, -dy);
		SetMatrix(im, 2, 3, -dz);
		Transform(m, im);
	}
	virtual HitRecord Hit(const Ray& ray) override
	{
		Ray invRay(MatrixMulPoint(invMatrix, ray.Origin), MatrixMulVector(invMatrix, ray.Direction));
		return objectPtr->Hit(invRay);
	}
};
