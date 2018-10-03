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
};

class MultiJittered : public Sampler
{
public:
    virtual ~MultiJittered() {}
    inline std::vector<Point2D> SampleSquare(int count)
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
    
    inline std::vector<Point2D> SampleCircle(int count)
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
                auto spx = 2.0 * (float (i * n + j) * subCellWidth + random.nextDouble() * subCellWidth) - 1.0;
                auto spy = 2.0 * (float (j * n + i) * subCellWidth + random.nextDouble() * subCellWidth) - 1.0;
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
};
