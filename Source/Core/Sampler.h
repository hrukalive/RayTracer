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
    int numSamples;
    int numSets;
    std::vector<Point2D> samples;
    std::vector<int> shuffledIndices;
    unsigned long count = 0;
    int jump = 0;
    CriticalSection cs;

    virtual void generate() = 0;
    void setupShuffledIndices();
public:
    Sampler();
    Sampler(int numSamples);
    Sampler(int numSamples, int numSets);
    virtual ~Sampler() {}
    virtual std::vector<Point2D> SampleSquare(int count) = 0;
    virtual std::vector<Point2D> SampleCircle(int count) = 0;
    virtual std::vector<Point3D> SampleHemisphere(int count, const FP_TYPE e) = 0;

    Point2D SampleSquareSingle();
    Point2D SampleCircleSingle();
    Point3D SampleHemisphereSingle(const FP_TYPE e);
};

class PreviewSampler : public Sampler
{
    void generate() override {}
public:
    virtual ~PreviewSampler() = default;
    std::vector<Point2D> SampleSquare(int count) override;
    std::vector<Point2D> SampleCircle(int count) override;
    std::vector<Point3D> SampleHemisphere(int count, const FP_TYPE e) override;
};

class MultiJittered : public Sampler
{
    void generate() override;
public:
    MultiJittered() : Sampler() { generate(); setupShuffledIndices(); }
    MultiJittered(int numSamples) : Sampler(numSamples) { generate(); setupShuffledIndices(); }
    MultiJittered(int numSamples, int numSets) : Sampler(numSamples, numSets) { generate(); setupShuffledIndices(); }
    virtual ~MultiJittered() = default;
    std::vector<Point2D> SampleSquare(int count) override;
    std::vector<Point2D> SampleCircle(int count) override;
    std::vector<Point3D> SampleHemisphere(int count, const FP_TYPE e) override;
};
