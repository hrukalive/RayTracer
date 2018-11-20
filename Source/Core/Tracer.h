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
public:
    virtual ~Tracer() = default;
    virtual std::pair<bool, RGBColor> Trace(const Ray& ray, int depth) const = 0;
    virtual std::pair<bool, RGBColor> Trace(const Ray& ray) const;
};

class RayCast : public Tracer
{
public:
    RayCast();
    virtual ~RayCast() = default;
    virtual std::pair<bool, RGBColor> Trace(const Ray& ray, int depth) const override;
};

class Whitted : public Tracer
{
    int maxDepth;
public:
    Whitted();
    Whitted(int maxDepth);
    virtual ~Whitted() = default;
    virtual std::pair<bool, RGBColor> Trace(const Ray& ray, int depth) const override;
};

class PathTrace : public Tracer
{
    int maxDepth;
public:
    PathTrace();
    PathTrace(int maxDepth);
    virtual ~PathTrace() = default;
    virtual std::pair<bool, RGBColor> Trace(const Ray& ray, int depth) const override;
};

class PhotonMapPreTrace : public Tracer
{
    int maxDepth;
public:
    PhotonMapPreTrace();
    PhotonMapPreTrace(int maxDepth);
    virtual ~PhotonMapPreTrace() = default;
    virtual std::pair<bool, RGBColor> Trace(const Ray& ray, int depth) const override;
};

class PhotonMapTrace : public Tracer
{
    int maxDepth;
public:
    PhotonMapTrace();
    PhotonMapTrace(int maxDepth);
    virtual ~PhotonMapTrace() = default;
    virtual std::pair<bool, RGBColor> Trace(const Ray& ray, int depth) const override;
};
