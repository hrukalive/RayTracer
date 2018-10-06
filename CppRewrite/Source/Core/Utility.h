/*
  ==============================================================================

    Utility.h
    Created: 28 Sep 2018 11:07:40am
    Author:  Bowen Sun

  ==============================================================================
*/

#pragma once

#include "../../JuceLibraryCode/JuceHeader.h"

#define FP_TYPE double
#define kEpsilon 0.001
#define kBBOXRatio 0.01
#define KBBOXMax 1.0
#define PI 3.14159265359
#define INV_PI 0.31830988618

typedef Vector3D<FP_TYPE> Vec3D;
typedef Vec3D Point3D;
typedef Vec3D RGBColor;
typedef Point<FP_TYPE> Point2D;
typedef dsp::Matrix<FP_TYPE> Matrix;

#define BLACK (RGBColor(0.0, 0.0, 0.0))
#define WHITE (RGBColor(1.0, 1.0, 1.0))
#define RED (RGBColor(1.0, 0.0, 0.0))

#define ZERO (Vec3D(0.0, 0.0, 0.0))
#define ONE (Vec3D(1.0, 1.0, 1.0))
#define X_DIR (Vec3D(1.0, 0.0, 0.0))
#define Y_DIR (Vec3D(0.0, 1.0, 0.0))
#define Z_DIR (Vec3D(0.0, 0.0, 1.0))

bool operator<(const Point3D& lhs, const Point3D& rhs);
std::ostream& operator<<(std::ostream& o, const Vec3D& vec);

inline Vec3D ElemMul(const Vec3D& a, const Vec3D& b)
{
	return Vec3D(a.x * b.x, a.y * b.y, a.z * b.z);
}

inline void SetMatrix(Matrix& m, const int row, const int col, const FP_TYPE val)
{
	FP_TYPE* data = m.getRawDataPointer();
	data[row * m.getNumColumns() + col] = val;
}

inline Point3D MatrixMulPoint(const Matrix& m, const Point3D& p)
{
	Matrix pm = Matrix(4, 1);
	SetMatrix(pm, 0, 0, p.x);
	SetMatrix(pm, 1, 0, p.y);
	SetMatrix(pm, 2, 0, p.z);
	SetMatrix(pm, 3, 0, 1.0);
	auto res = m * pm;
	auto ret = res.getRawDataPointer();
	return Point3D(ret[0] / ret[3], ret[1] / ret[3], ret[2] / ret[3]);
}

inline Point3D MatrixMulVector(const Matrix& m, const Vec3D& n)
{
	Matrix nm = Matrix(4, 1);
	SetMatrix(nm, 0, 0, n.x);
	SetMatrix(nm, 1, 0, n.y);
	SetMatrix(nm, 2, 0, n.z);
	SetMatrix(nm, 3, 0, 0.0);
	auto res = m * nm;
	auto ret = res.getRawDataPointer();
	return Point3D(ret[0], ret[1], ret[2]);
}

struct Ray
{
	Point3D Origin;
	Vec3D Direction;
	Ray() {}
    Ray(Vec3D origin, Vec3D direction) : Origin(origin), Direction(direction) {}
	inline Point3D GetPoint(FP_TYPE t) const
	{
		return Origin + (Direction * t);
	}
};

class Material;
class World;
struct HitRecord
{
	bool Hit = false;
	Vec3D Normal;
	Vec3D HitPoint;
	FP_TYPE T = 0.0f;
	Ray Ray;
    std::shared_ptr<Material> MaterialPtr = nullptr;
    std::shared_ptr<World> WorldPtr = nullptr;

	HitRecord() {}
	HitRecord(Vec3D normal, Vec3D hitPoint, FP_TYPE t)
		: Normal(normal), HitPoint(hitPoint), T(t) {}
};

