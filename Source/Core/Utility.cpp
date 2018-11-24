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

int SolveQuadric(double c[3], double s[2])
{
    double p, q, D;

    /* normal form: x^2 + px + q = 0 */

    p = c[1] / (2 * c[2]);
    q = c[0] / c[2];

    D = p * p - q;

    if (abs(D) < EQN_EPS) {
        s[0] = -p;
        return 1;
    }
    else if (D > 0) {
        double sqrt_D = sqrt(D);

        s[0] = sqrt_D - p;
        s[1] = -sqrt_D - p;
        return 2;
    }
    else /* if (D < 0) */
        return 0;
}

int SolveCubic(double c[4], double s[3])
{
    int     i, num;
    double  sub;
    double  A, B, C;
    double  sq_A, p, q;
    double  cb_p, D;

    /* normal form: x^3 + Ax^2 + Bx + C = 0 */

    A = c[2] / c[3];
    B = c[1] / c[3];
    C = c[0] / c[3];

    /*  substitute x = y - A/3 to eliminate quadric term:
    x^3 +px + q = 0 */

    sq_A = A * A;
    p = 1.0 / 3 * (-1.0 / 3 * sq_A + B);
    q = 1.0 / 2 * (2.0 / 27 * A * sq_A - 1.0 / 3 * A * B + C);

    /* use Cardano's formula */

    cb_p = p * p * p;
    D = q * q + cb_p;

    if (abs(D) < EQN_EPS) {
        if (abs(D) < EQN_EPS) { /* one triple solution */
            s[0] = 0;
            num = 1;
        }
        else { /* one single and one double solution */
            double u = cbrt(-q);
            s[0] = 2 * u;
            s[1] = -u;
            num = 2;
        }
    }
    else if (D < 0) { /* Casus irreducibilis: three real solutions */
        double phi = 1.0 / 3 * acos(-q / sqrt(-cb_p));
        double t = 2 * sqrt(-p);

        s[0] = t * cos(phi);
        s[1] = -t * cos(phi + PI / 3);
        s[2] = -t * cos(phi - PI / 3);
        num = 3;
    }
    else { /* one real solution */
        double sqrt_D = sqrt(D);
        double u = cbrt(sqrt_D - q);
        double v = -cbrt(sqrt_D + q);

        s[0] = u + v;
        num = 1;
    }

    /* resubstitute */

    sub = 1.0 / 3 * A;

    for (i = 0; i < num; ++i)
        s[i] -= sub;

    return num;
}

int SolveQuartic(double c[5], double s[4])
{
    double  coeffs[4];
    double  z, u, v, sub;
    double  A, B, C, D;
    double  sq_A, p, q, r;
    int     i, num;

    /* normal form: x^4 + Ax^3 + Bx^2 + Cx + D = 0 */

    A = c[3] / c[4];
    B = c[2] / c[4];
    C = c[1] / c[4];
    D = c[0] / c[4];

    /*  substitute x = y - A/4 to eliminate cubic term:
    x^4 + px^2 + qx + r = 0 */

    sq_A = A * A;
    p = -3.0 / 8 * sq_A + B;
    q = 1.0 / 8 * sq_A * A - 1.0 / 2 * A * B + C;
    r = -3.0 / 256 * sq_A*sq_A + 1.0 / 16 * sq_A*B - 1.0 / 4 * A*C + D;

    if (abs(r) < EQN_EPS) {
        /* no absolute term: y(y^3 + py + q) = 0 */

        coeffs[0] = q;
        coeffs[1] = p;
        coeffs[2] = 0;
        coeffs[3] = 1;

        num = SolveCubic(coeffs, s);

        s[num++] = 0;
    }
    else {
        /* solve the resolvent cubic ... */

        coeffs[0] = 1.0 / 2 * r * p - 1.0 / 8 * q * q;
        coeffs[1] = -r;
        coeffs[2] = -1.0 / 2 * p;
        coeffs[3] = 1;

        (void)SolveCubic(coeffs, s);

        /* ... and take the one real solution ... */

        z = s[0];

        /* ... to build two quadric equations */

        u = z * z - r;
        v = 2 * z - p;

        if (abs(D) < EQN_EPS)
            u = 0;
        else if (u > 0)
            u = sqrt(u);
        else
            return 0;

        if (abs(D) < EQN_EPS)
            v = 0;
        else if (v > 0)
            v = sqrt(v);
        else
            return 0;

        coeffs[0] = z - u;
        coeffs[1] = q < 0 ? -v : v;
        coeffs[2] = 1;

        num = SolveQuadric(coeffs, s);

        coeffs[0] = z + u;
        coeffs[1] = q < 0 ? v : -v;
        coeffs[2] = 1;

        num += SolveQuadric(coeffs, s + num);
    }

    /* resubstitute */

    sub = 1.0 / 4 * A;

    for (i = 0; i < num; ++i)
        s[i] -= sub;

    return num;
}
