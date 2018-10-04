/*
  ==============================================================================

    Plane.h
    Created: 28 Sep 2018 11:00:58am
    Author:  Bowen Sun

  ==============================================================================
*/

#pragma once

class InfPlane : public GeometricObject
{
	Point3D a;
	Vec3D n;
public:
	InfPlane(Point3D origin, Vec3D normal) : a(origin), n(normal) {}
    virtual ~InfPlane() {}
	void setOrigin(Point3D origin) { a = origin; }
	void setNormal(Vec3D normal) { n = normal; }
	Point3D getOrigin() { return a; }
	Vec3D getNormal() { return n.normalised(); }

	virtual HitRecord Hit(const Ray& ray) override
	{
		HitRecord record;
		FP_TYPE t = ((a - ray.Origin) * n) / (ray.Direction * n);
		if (t > kEpsilon)
		{
			record.Hit = true;
			record.Normal = n.normalised();
			record.HitPoint = ray.GetPoint(t);
            record.MaterialPtr = materialPtr;
			record.T = t;
		}
		return record;
	}
};

class Plane : public GeometricObject
{
	Point3D a;
	Vec3D u, v, n;
public:
	Plane(Point3D origin, Vec3D uDirection, Vec3D vDirection)
		: a(origin), u(uDirection), v(vDirection)
	{
		n = u ^ v;
	}
    virtual ~Plane() {}

	void setOrigin(Point3D origin) { a = origin; }
	void setU(Vec3D uDirection) { u = uDirection; n = u ^ v; }
	void setV(Vec3D vDirection) { v = vDirection; n = u ^ v; }
	Point3D getOrigin() { return a; }
	Vec3D getU() { return u; }
	Vec3D getV() { return v; }
	Vec3D getNormal() { return n.normalised(); }

	virtual HitRecord Hit(const Ray& ray) override
	{
		HitRecord record;
		FP_TYPE t = ((a - ray.Origin) * n) / (ray.Direction * n);
		auto p = ray.GetPoint(t);
		auto tu = (p - a) * u.normalised() / u.length();
		auto tv = (p - a) * v.normalised() / v.length();

		if (t > kEpsilon && tu >= 0.0 && tu <= 1.0 && tv >= 0.0 && tv <= 1.0)
		{
			record.Hit = true;
			record.Normal = n.normalised();
			record.HitPoint = p;
            record.MaterialPtr = materialPtr;
			record.T = t;
		}
		return record;
	}
};
