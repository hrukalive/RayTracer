#include "Sampler.h"

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
