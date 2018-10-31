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

public:
    Tracer(std::shared_ptr<World>& world);
    virtual ~Tracer() = default;
    virtual RGBColor Trace(const Ray& ray, int depth) const = 0;
    virtual RGBColor Trace(const Ray& ray) const;
};

class RayCast : public Tracer
{
public:
    RayCast(std::shared_ptr<World>& world);
    virtual ~RayCast() = default;
    virtual RGBColor Trace(const Ray& ray, int depth) const override;
};

class Whitted : public Tracer
{
    int maxDepth;
public:
    Whitted(std::shared_ptr<World>& world);
    Whitted(std::shared_ptr<World>& world, int maxDepth);
    virtual ~Whitted() = default;
    virtual RGBColor Trace(const Ray& ray, int depth) const override;
};
