/*
  ==============================================================================

    Sampler.h
    Created: 28 Sep 2018 11:04:10am
    Author:  Bowen Sun

  ==============================================================================
*/

#pragma once
#include "Utility.h"
#include "Viewplane.h"

class Sampler
{
protected:
    Random random;
public:
    virtual ~Sampler() {}
    virtual std::vector<Point2D> SampleSquare(int count) = 0;
    virtual std::vector<Point2D> SampleCircle(int count) = 0;
    virtual std::vector<Point3D> SampleHemisphere(int count, const FP_TYPE e) = 0;
};

class PreviewSampler : public Sampler
{
public:
    virtual ~PreviewSampler() = default;
    std::vector<Point2D> SampleSquare(int count) override;
    std::vector<Point2D> SampleCircle(int count) override;
    std::vector<Point3D> SampleHemisphere(int count, const FP_TYPE e) override;
};

class MultiJittered : public Sampler
{
public:
    virtual ~MultiJittered() = default;
    std::vector<Point2D> SampleSquare(int count) override;
    std::vector<Point2D> SampleCircle(int count) override;
    std::vector<Point3D> SampleHemisphere(int count, const FP_TYPE e) override;
};
