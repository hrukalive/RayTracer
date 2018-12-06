/*
  ==============================================================================

    Sampler.cpp
    Created: 28 Sep 2018 11:04:10am
    Author:  Bowen Sun

  ==============================================================================
*/

#include "Sampler.h"
#include <algorithm>

Sampler::Sampler() : numSamples(16), numSets(32)
{
    samples.reserve(numSamples * numSets);
}
Sampler::Sampler(int numSamples) : numSamples(numSamples), numSets(32)
{
    samples.reserve(numSamples * numSets);
}
Sampler::Sampler(int numSamples, int numSets) : numSamples(numSamples), numSets(numSets)
{
    samples.reserve(numSamples * numSets);
}

void Sampler::setupShuffledIndices()
{
    shuffledIndices.reserve(numSamples * numSets);
    std::vector<int> indices;
    for (int i = 0; i < numSamples; i++)
        indices.push_back(i);
    for (int p = 0; p < numSets; p++)
    {
        random_shuffle(indices.begin(), indices.end());
        for (int i = 0; i < numSamples; i++)
            shuffledIndices.push_back(indices[i]);
    }
}

Point2D Sampler::SampleSquareSingle()
{
    cs.enter();
    if (count % numSamples == 0)
    {
        jump = (unsigned long)random.nextInt((int)numSets) * numSamples;
        count = 0;
    }
    auto ret = samples[jump + shuffledIndices[jump + count++ % numSamples]];
    cs.exit();
    return ret;
}

Point2D Sampler::SampleCircleSingle()
{
    Point2D p = SampleSquareSingle();
    auto spx = p.getX(), spy = p.getY();
    if (spx > -spy)
    {
        if (spx > spy)
        {
            auto r = spx;
            auto phi = spy / spx * PI / 4.0;
            return Point2D(r * cos(phi), r * sin(phi));
        }
        else
        {
            auto r = spy;
            auto phi = (2.0 - spx / spy) * PI / 4.0;
            return Point2D(r * cos(phi), r * sin(phi));
        }
    }
    else
    {
        if (spx < spy)
        {
            auto r = -spx;
            auto phi = (4.0 + spy / spx) * PI / 4.0;
            return Point2D(r * cos(phi), r * sin(phi));
        }
        else
        {
            auto r = -spy;
            if (spy != 0.0)
            {
                auto phi = (6.0 - spx / spy) * PI / 4.0;
                return Point2D(r * cos(phi), r * sin(phi));
            }
            else
            {
                return Point2D(r, 0.0);
            }
        }
    }
}

Point3D Sampler::SampleHemisphereSingle(const FP_TYPE e)
{
    Point2D p = SampleSquareSingle();
    auto spx = p.getX(), spy = p.getY();
    FP_TYPE cos_phi = cos(2.0 * PI * spx);
    FP_TYPE sin_phi = sin(2.0 * PI * spx);
    FP_TYPE cos_theta = pow((1.0 - spy), 1.0 / (e + 1.0));
    FP_TYPE sin_theta = sqrt(1.0 - cos_theta * cos_theta);
    return Point3D(sin_theta * cos_phi, sin_theta * sin_phi, cos_theta);
}

std::vector<Point2D> PreviewSampler::SampleSquare(int count)
{
    std::vector<Point2D> ret;
    ret.push_back(Point2D(0.5, 0.5));
    return ret;
}
std::vector<Point2D> PreviewSampler::SampleCircle(int count)
{
    return SampleSquare(count);
}

std::vector<Point3D> PreviewSampler::SampleHemisphere(int count, const FP_TYPE e)
{
    std::vector<Point3D> ret;
    ret.push_back(Point3D(0.0, 0.0, 1.0));
    return ret;
}

std::shared_ptr<Sampler> Sampler::parse(StringArray& cmd, std::unordered_map<String, std::shared_ptr<void>>& env)
{
    int numSamples = std::stoi(cmd[2].toStdString());
    int numSets = std::stoi(cmd[3].toStdString());
    std::shared_ptr<Sampler> ret = nullptr;
    if (cmd[1] == "PreviewSampler")
        ret.reset(new PreviewSampler());
    else if (cmd[1] == "MultiJittered")
        ret.reset(new MultiJittered(numSamples, numSets));
    else if (cmd[1] == "Hammersley")
        ret.reset(new Hammersley(numSamples, numSets));
    else
        throw std::invalid_argument("[SAMPLER] Type not found.");
    return ret;
}
