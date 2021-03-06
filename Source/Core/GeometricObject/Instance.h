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
    void Transform(const Matrix& transform, const Matrix& invTransform);
protected:
    void UpdateBoundingBox() override;
public:
    Instance() = default;
    Instance(std::shared_ptr<GeometricObject> objectPtr);
    virtual ~Instance() = default;
    void SetObject(std::shared_ptr<GeometricObject> newObjectPtr);

    void Translate(const FP_TYPE dx, const FP_TYPE dy, const FP_TYPE dz);
    void Scale(const FP_TYPE a, const FP_TYPE b, const FP_TYPE c);
    void RotateX(const FP_TYPE radian);
    void RotateY(const FP_TYPE radian);
    void RotateZ(const FP_TYPE radian);

    Matrix getInvMatrix() { return invMatrix; }

    virtual HitRecord Hit(const Ray& ray) override;

    static std::shared_ptr<GeometricObject> parse(StringArray& cmd, std::unordered_map<String, std::shared_ptr<void>>& env);
};
