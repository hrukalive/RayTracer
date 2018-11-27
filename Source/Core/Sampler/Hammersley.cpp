#include "Sampler.h"

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
