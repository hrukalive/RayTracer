/*
  ==============================================================================

    Tracer.h
    Created: 28 Sep 2018 11:04:29am
    Author:  Bowen Sun

  ==============================================================================
*/

#pragma once
#include "Utility.h"
#include "World.h"

class Tracer
{
protected:
	std::shared_ptr<World> worldPtr;

	virtual RGBColor Trace(const Ray& ray, int depth) const = 0;
public:
	Tracer(std::shared_ptr<World>& world) : worldPtr(world) {}
	virtual RGBColor Trace(const Ray& ray) const
	{
		return Trace(ray, 0);
	}
};

class RayCast : public Tracer
{
private:
	virtual RGBColor Trace(const Ray& ray, int depth) const
	{
		HitRecord record = worldPtr->HitObjects(ray);
		return record.Hit ? record.MaterialPtr->Shade(record) : worldPtr->GetBackgroundColor();
	}
};
