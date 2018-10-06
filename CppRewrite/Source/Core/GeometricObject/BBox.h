/*
  ==============================================================================

    BBox.h
    Created: 4 Oct 2018 2:52:52pm
    Author:  Bowen Sun

  ==============================================================================
*/

#pragma once

#include "IObject.h"
#include "../Materials/Material.h"

class BBox : public IObject
{
	Point3D bboxMin, bboxMax;
	FP_TYPE diagLength = 0.0;
public:
	BBox()
	{
		SetBoundingBox(ZERO, ZERO);
	}
    BBox(const Point3D min, const Point3D max)
    {
        SetBoundingBox(min, max);
    }
    virtual ~BBox() {}
    void SetBoundingBox(const Point3D min, const Point3D max)
    {
		this->bboxMin = min;
		this->bboxMax = max;
		diagLength = (bboxMax - bboxMin).length();
    }
	void Merge(const BBox& other)
	{
		SetBoundingBox(Point3D(fmin(bboxMin.x, other.bboxMin.x), fmin(bboxMin.y, other.bboxMin.y), fmin(bboxMin.z, other.bboxMin.z)), 
			Point3D(fmax(bboxMax.x, other.bboxMax.x), fmax(bboxMax.y, other.bboxMax.y), fmax(bboxMax.z, other.bboxMax.z)));
	}
	Point3D GetMinPoint() { return bboxMin; }
	Point3D GetMaxPoint() { return bboxMax; }
    Vec3D GetNormal(const int face) const
    {
        switch (face)
        {
            case 0: return -X_DIR;
            case 1: return -Y_DIR;
            case 2: return -Z_DIR;
            case 3: return X_DIR;
            case 4: return Y_DIR;
            case 5: return Z_DIR;
            default: return ZERO;
        }
    }
    virtual HitRecord Hit(const Ray& ray) override
    {
        auto& rayOrigin = ray.Origin;
        auto& rayDir = ray.Direction;
		auto delta = (diagLength * kBBOXRatio > KBBOXMax ? KBBOXMax : (diagLength * kBBOXRatio > KBBOXMax));
        
		FP_TYPE xMin = bboxMin.x, yMin = bboxMin.y, zMin = bboxMin.z, xMax = bboxMax.x, yMax = bboxMax.y, zMax = bboxMax.z;
        FP_TYPE txMin, txMax, tyMin, tyMax, tzMin, tzMax;
        
        FP_TYPE a = 1.0 / rayDir.x;
        txMin = (xMin - delta - rayOrigin.x) * a;
        txMax = (xMax + delta - rayOrigin.x) * a;
        if (a < 0.0)
        {
            std::swap(txMin, txMax);
        }
        
        FP_TYPE b = 1.0 / rayDir.y;
        tyMin = (yMin - delta - rayOrigin.y) * b;
        tyMax = (yMax + delta - rayOrigin.y) * b;
        if (b < 0.0)
        {
            std::swap(tyMin, tyMax);
        }
        
        FP_TYPE c = 1.0 / rayDir.z;
        tzMin = (zMin - delta - rayOrigin.z) * c;
        tzMax = (zMax + delta - rayOrigin.z) * c;
        if (c < 0.0)
        {
            std::swap(tzMin, tzMax);
        }
        
        FP_TYPE t0, t1;
        int faceIn, faceOut;
        if (txMin > tyMin)
        {
            t0 = txMin;
            faceIn = (a >= 0.0) ? 0 : 3;
        }
        else
        {
            t0 = tyMin;
            faceIn = (b >= 0.0) ? 1 : 4;
        }
        if (tzMin > t0)
        {
            t0 = tzMin;
            faceIn = (c >= 0.0) ? 2 : 5;
        }
        
        if (txMax < tyMax)
        {
            t1 = txMax;
            faceOut = (a >= 0.0) ? 3 : 0;
        }
        else
        {
            t1 = tyMax;
            faceOut = (b >= 0.0) ? 4 : 1;
        }
        if (tzMax < t1)
        {
            t1 = tzMax;
            faceOut = (c >= 0.0) ? 5 : 2;
        }
        
        HitRecord record;
		std::shared_ptr<Material> tmp{ new ConstColor() };

        if (t0 < t1 && t1 > kEpsilon)
        {
            if (t0 > kEpsilon)
            {
                record.Hit = true;
                record.Normal = GetNormal(faceIn);
                record.HitPoint = ray.GetPoint(t0);
				record.MaterialPtr = tmp;
                record.Ray = ray;
                record.T = t0;
            }
            else
            {
                record.Hit = true;
                record.Normal = GetNormal(faceOut);
                record.HitPoint = ray.GetPoint(t1);
				record.MaterialPtr = tmp;
                record.Ray = ray;
                record.T = t1;
            }
        }
        return record;
    }
};
