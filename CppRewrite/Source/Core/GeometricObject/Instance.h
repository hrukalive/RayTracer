/*
  ==============================================================================

    Instance.h
    Created: 4 Oct 2018 4:04:36pm
    Author:  Bowen Sun

  ==============================================================================
*/

#pragma once
#include "GeometricObject.h"
#include <sstream>

class Instance : public GeometricObject
{
	std::shared_ptr<GeometricObject> objectPtr;
	Matrix forwardMatrix = Matrix::identity(4);
	Matrix invMatrix = Matrix::identity(4);
	Matrix invMatrixT = Matrix::identity(4);
	void Transform(const Matrix& transform, const Matrix& invTransform)
	{
		forwardMatrix = transform * forwardMatrix;
		invMatrix = invMatrix * invTransform;
		invMatrixT = MatrixTranspose(invMatrix);

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
		m(0, 3) = dx;
		m(1, 3) = dy;
		m(2, 3) = dz;
		auto im = Matrix::identity(4);
		im(0, 3) = -dx;
		im(1, 3) = -dy;
		im(2, 3) = -dz;
		Transform(m, im);
	}
	void Scale(const FP_TYPE a, const FP_TYPE b, const FP_TYPE c)
	{
		auto m = Matrix::identity(4);
		m(0, 0) = a;
		m(1, 1) = b;
		m(2, 2) = c;
		auto im = Matrix::identity(4);
		im(0, 0) = 1.0 / a;
		im(1, 1) = 1.0 / b;
		im(2, 2) = 1.0 / c;
		Transform(m, im);
	}
	virtual HitRecord Hit(const Ray& ray) override
	{
		Ray invRay(MatrixMulPoint(invMatrix, ray.Origin), MatrixMulVector(invMatrix, ray.Direction));
		HitRecord record = objectPtr->Hit(invRay);
		record.Ray = ray;
		record.HitPoint = ray.GetPoint(record.T);
		record.Normal = MatrixMulVector(invMatrixT, record.Normal).normalised();
		return record;
	}
};
