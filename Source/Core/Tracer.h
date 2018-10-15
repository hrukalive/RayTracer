/*
  ==============================================================================

    Tracer.h
    Created: 28 Sep 2018 11:04:29am
    Author:  Bowen Sun

  ==============================================================================
*/

#pragma once
#include "Utility.h"
#include "Materials/Material.h"
#include "World.h"

class Tracer
{
protected:
	std::shared_ptr<World> worldPtr;

	virtual RGBColor Trace(const Ray& ray, int depth) const = 0;
public:
    Tracer(std::shared_ptr<World>& world);
    virtual ~Tracer() = default;
    virtual RGBColor Trace(const Ray& ray) const;
};

class RayCast : public Tracer
{
protected:
    virtual RGBColor Trace(const Ray& ray, int depth) const override;
public:
    RayCast(std::shared_ptr<World>& world);
    virtual ~RayCast() = default;
};
