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

std::vector<Point2D> MultiJittered::SampleSquare(int count)
{
    auto n = int(sqrt(count));
    auto subCellWidth = 1.0 / count;
    Random shuffleRand;

    std::vector<Point2D> ret;
    std::vector<double> sampleX, sampleY;

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            sampleX.push_back((i * n + j) * subCellWidth + random.nextDouble() * subCellWidth);
            sampleY.push_back((j * n + i) * subCellWidth + random.nextDouble() * subCellWidth);
        }
    }
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            int k = shuffleRand.nextInt(Range<int>(j, n));
            std::swap(sampleX[i * n + j], sampleX[i * n + k]);
        }
    }
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            int k = shuffleRand.nextInt(Range<int>(j, n));
            std::swap(sampleY[j * n + i], sampleY[k * n + i]);
        }
    }
    for (int i = 0; i < n * n; i++)
    {
        ret.push_back(Point2D(sampleX[i], sampleY[i]));
    }
    return ret;
}

std::vector<Point3D> MultiJittered::SampleHemisphere(int count, const FP_TYPE e)
{
    auto n = int(sqrt(count));
    auto subCellWidth = 1.0 / count;
    Random shuffleRand;
    
    std::vector<Point3D> ret;
    std::vector<double> sampleX, sampleY;
    
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            sampleX.push_back((i * n + j) * subCellWidth + random.nextDouble() * subCellWidth);
            sampleY.push_back((j * n + i) * subCellWidth + random.nextDouble() * subCellWidth);
        }
    }
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            int k = shuffleRand.nextInt(Range<int>(j, n));
            std::swap(sampleX[i * n + j], sampleX[i * n + k]);
        }
    }
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            int k = shuffleRand.nextInt(Range<int>(j, n));
            std::swap(sampleY[j * n + i], sampleY[k * n + i]);
        }
    }
    for (int i = 0; i < n * n; i++)
    {
        FP_TYPE cos_phi = cos(2.0 * PI * sampleX[i]);
        FP_TYPE sin_phi = sin(2.0 * PI * sampleX[i]);
        FP_TYPE cos_theta = pow((1.0 - sampleY[i]), 1.0 / (e + 1.0));
        FP_TYPE sin_theta = sqrt(1.0 - cos_theta * cos_theta);
        ret.push_back(Point3D(sin_theta * cos_phi, sin_theta * sin_phi, cos_theta));
    }
    return ret;
}

std::vector<Point2D> MultiJittered::SampleCircle(int count)
{
    auto n = int(sqrt(count));
    auto subCellWidth = 1.0 / count;
    Random shuffleRand;

    std::vector<Point2D> ret;
    std::vector<double> sampleX, sampleY;

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            auto spx = 2.0 * (float(i * n + j) * subCellWidth + random.nextDouble() * subCellWidth) - 1.0;
            auto spy = 2.0 * (float(j * n + i) * subCellWidth + random.nextDouble() * subCellWidth) - 1.0;
            if (spx > -spy)
            {
                if (spx > spy)
                {
                    auto r = spx;
                    auto phi = spy / spx * PI / 4.0;
                    sampleX.push_back(r * cos(phi));
                    sampleY.push_back(r * sin(phi));
                }
                else
                {
                    auto r = spy;
                    auto phi = (2.0 - spx / spy) * PI / 4.0;
                    sampleX.push_back(r * cos(phi));
                    sampleY.push_back(r * sin(phi));
                }
            }
            else
            {
                if (spx < spy)
                {
                    auto r = -spx;
                    auto phi = (4.0 + spy / spx) * PI / 4.0;
                    sampleX.push_back(r * cos(phi));
                    sampleY.push_back(r * sin(phi));
                }
                else
                {
                    auto r = -spy;
                    if (spy != 0.0)
                    {
                        auto phi = (6.0 - spx / spy) * PI / 4.0;
                        sampleX.push_back(r * cos(phi));
                        sampleY.push_back(r * sin(phi));
                    }
                    else
                    {
                        sampleX.push_back(r);
                        sampleY.push_back(0.0);
                    }
                }
            }
        }
    }
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            int k = shuffleRand.nextInt(Range<int>(j, n));
            std::swap(sampleX[i * n + j], sampleX[i * n + k]);
        }
    }
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            int k = shuffleRand.nextInt(Range<int>(j, n));
            std::swap(sampleY[j * n + i], sampleY[k * n + i]);
        }
    }
    for (int i = 0; i < n * n; i++)
    {
        ret.push_back(Point2D(sampleX[i], sampleY[i]));
    }
    return ret;
}

void MultiJittered::generate()
{
    auto n = int(sqrt(numSamples));
    auto subCellWidth = 1.0 / numSamples;
    Random shuffleRand;

    std::vector<double> sampleX, sampleY;

    for (int k = 0; k < numSets; k++)
    {
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                sampleX.push_back((i * n + j) * subCellWidth + random.nextDouble() * subCellWidth);
                sampleY.push_back((j * n + i) * subCellWidth + random.nextDouble() * subCellWidth);
            }
        }
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                int k = shuffleRand.nextInt(Range<int>(j, n));
                std::swap(sampleX[i * n + j], sampleX[i * n + k]);
            }
        }
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                int k = shuffleRand.nextInt(Range<int>(j, n));
                std::swap(sampleY[j * n + i], sampleY[k * n + i]);
            }
        }
        for (int i = 0; i < n * n; i++)
            samples.push_back(Point2D(sampleX[i], sampleY[i]));
    }
    numSamples = samples.size() / numSets;
}

void Hammersley::generate()
{
    for (int k = 0; k < numSets; k++)
        for (int k = 0; k < numSamples; k++) {
            FP_TYPE x = 0;
            int kk = k;
            for (FP_TYPE p = 0.5; kk; p *= 0.5, kk >>= 1) {
                // kk mod 2 == 1
                if (kk & 1)
                    x += p;
            }
            FP_TYPE y = (k + 0.5) / numSamples;
            samples.push_back(Point2D(x, y));
        }
    numSamples = samples.size() / numSets;
}

std::vector<Point2D> Hammersley::SampleSquare(int count)
{
    std::vector<Point2D> ret;
    for (int k = 0; k < count; k++) {
        FP_TYPE x = 0;
        int kk = k;
        for (FP_TYPE p = 0.5; kk; p *= 0.5, kk >>= 1) {
            // kk mod 2 == 1
            if (kk & 1)
                x += p;
        }
        FP_TYPE y = (k + 0.5) / count;
        ret.push_back(Point2D(x, y));
    }
    return ret;
}

std::vector<Point3D> Hammersley::SampleHemisphere(int count, const FP_TYPE e)
{
    std::vector<Point3D> ret;

    for (int k = 0; k < count; k++)
    {
        FP_TYPE x = 0;
        int kk = k;
        for (FP_TYPE p = 0.5; kk; p *= 0.5, kk >>= 1) {
            // kk mod 2 == 1
            if (kk & 1)
                x += p;
        }
        FP_TYPE y = (k + 0.5) / count;

        FP_TYPE cos_phi = cos(TWO_PI * x);
        FP_TYPE sin_phi = sin(TWO_PI * x);
        FP_TYPE cos_theta = pow((1.0 - y), 1.0 / (e + 1.0));
        FP_TYPE sin_theta = sqrt(1.0 - cos_theta * cos_theta);
        ret.push_back(Point3D(sin_theta * cos_phi, sin_theta * sin_phi, cos_theta));
    }
    return ret;
}

std::vector<Point2D> Hammersley::SampleCircle(int count)
{
    std::vector<Point2D> ret;

    for (int k = 0; k < count; k++)
    {
        FP_TYPE spx = 0;
        int kk = k;
        for (FP_TYPE p = 0.5; kk; p *= 0.5, kk >>= 1) {
            // kk mod 2 == 1
            if (kk & 1)
                spx += p;
        }
        FP_TYPE spy = (k + 0.5) / count;
        if (spx > -spy)
        {
            if (spx > spy)
            {
                auto r = spx;
                auto phi = spy / spx * PI / 4.0;
                ret.push_back(Point2D(r * cos(phi), r * sin(phi)));
            }
            else
            {
                auto r = spy;
                auto phi = (2.0 - spx / spy) * PI / 4.0;
                ret.push_back(Point2D(r * cos(phi), r * sin(phi)));
            }
        }
        else
        {
            if (spx < spy)
            {
                auto r = -spx;
                auto phi = (4.0 + spy / spx) * PI / 4.0;
                ret.push_back(Point2D(r * cos(phi), r * sin(phi)));
            }
            else
            {
                auto r = -spy;
                if (spy != 0.0)
                {
                    auto phi = (6.0 - spx / spy) * PI / 4.0;
                    ret.push_back(Point2D(r * cos(phi), r * sin(phi)));
                }
                else
                {
                    ret.push_back(Point2D(r, 0.0));
                }
            }
        }
    }
    return ret;
}
