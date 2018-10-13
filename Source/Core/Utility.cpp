/*
  ==============================================================================

    Utility.cpp
    Created: 6 Oct 2018 4:29:50pm
    Author:  bowen

  ==============================================================================
*/

#include "Utility.h"

std::ostream& operator<<(std::ostream& o, const Vec3D& vec)
{
    o << vec.x << ", " << vec.y << ", " << vec.z;
    return o;
}

std::ostream& operator<<(std::ostream& o, const Matrix& m)
{
	o << m(0, 0) << " " << m(0, 1) << " " << m(0, 2) << " " << m(0, 3) << std::endl;
	o << m(1, 0) << " " << m(1, 1) << " " << m(1, 2) << " " << m(1, 3) << std::endl;
	o << m(2, 0) << " " << m(2, 1) << " " << m(2, 2) << " " << m(2, 3) << std::endl;
	o << m(3, 0) << " " << m(3, 1) << " " << m(3, 2) << " " << m(3, 3);
	return o;
}

bool operator<(const Point3D& lhs, const Point3D& rhs)
{
	return lhs.x < rhs.x && lhs.y < rhs.y && lhs.z < rhs.z;
}

std::vector<FP_TYPE> bilaterialFilter(const std::vector<FP_TYPE>& I, int width, int height)
{
    auto gaussian = [](FP_TYPE x, FP_TYPE sigma) { return exp(-(x * x) / (2 * sigma * sigma)) / (2 * PI * sigma * sigma); };
    auto dist = [](int x, int y, int i, int j) { return sqrt(pow(x - i, 2) + pow(y - j, 2)); };
    auto Imin = *std::min_element(I.begin(), I.end());
    auto Imax = *std::max_element(I.begin(), I.end());
    auto Idelta = Imax - Imin;
    auto sigmaSpatial = std::max(3, std::min(width, height) / 16);
    auto sigmaRange = std::max(3, (int)ceil(0.1 * Idelta));
    auto kernelWidth = std::max(2 * sigmaSpatial + 1, 2 * sigmaRange + 1);
    auto halfKernelWidth = kernelWidth / 2;

    std::vector<FP_TYPE> ret;
    ret.resize(I.size());
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            auto centerValue = I[y * width + x];
            auto response = 0.0;
            auto normalizer = 0.0;
            for (int j = -halfKernelWidth; j <= halfKernelWidth; j++)
            {
                for (int i = -halfKernelWidth; i <= halfKernelWidth; i++)
                {
                    if (y + j >= 0 && y + j < height && x + i >= 0 && x + i < width)
                    {
                        int neighborValue = I[(y + j) * width + x + i];
                        auto gi = gaussian(neighborValue - centerValue, sigmaRange);
                        auto gd = gaussian(dist(x, y, x + i, y + j), sigmaSpatial);
                        auto w = gi * gd;
                        response += neighborValue * w;
                        normalizer += w;
                    }
                }
            }
            ret[y * width + x] = response / normalizer;
        }
    }
    return ret;
}
