/*
  ==============================================================================

    Torus.cpp
    Created: 22 Nov 2018 12:49:17am
    Author:  bowen

  ==============================================================================
*/

#include "Torus.h"

void Torus::UpdateBoundingBox()
{
    boundingBox = BBox(Point3D(-a - b, -b, -a - b), Point3D(a + b, b, a + b));
}

Torus::Torus(FP_TYPE a, FP_TYPE b) : a(a), b(b) { UpdateBoundingBox(); }

HitRecord Torus::Hit(const Ray& ray)
{
    if (!boundingBox.Hit(ray).Hit)
        return HitRecord();

    double x1 = ray.Origin.x;
    double y1 = ray.Origin.y;
    double z1 = ray.Origin.z;
    double d1 = ray.Direction.x;
    double d2 = ray.Direction.y;
    double d3 = ray.Direction.z;

    double coeffs[5];	// coefficient array for the quartic equation
    double roots[4];	// solution array for the quartic equation

                        // define the coefficients of the quartic equation

    double sum_d_sqrd = d1 * d1 + d2 * d2 + d3 * d3;
    double e = x1 * x1 + y1 * y1 + z1 * z1 - a * a - b * b;
    double f = x1 * d1 + y1 * d2 + z1 * d3;
    double four_a_sqrd = 4.0 * a * a;

    coeffs[0] = e * e - four_a_sqrd * (b * b - y1 * y1); 	// constant term
    coeffs[1] = 4.0 * f * e + 2.0 * four_a_sqrd * y1 * d2;
    coeffs[2] = 2.0 * sum_d_sqrd * e + 4.0 * f * f + four_a_sqrd * d2 * d2;
    coeffs[3] = 4.0 * sum_d_sqrd * f;
    coeffs[4] = sum_d_sqrd * sum_d_sqrd;  					// coefficient of t^4

                                                            // find roots of the quartic equation

    int num_real_roots = SolveQuartic(coeffs, roots);

    bool	intersected = false;
    double 	t = kHugeValue;

    if (num_real_roots == 0)  // ray misses the torus
        return HitRecord();

    // find the smallest root greater than kEpsilon, if any
    // the roots array is not sorted

    for (int j = 0; j < num_real_roots; j++)
        if (roots[j] > kEpsilon) {
            intersected = true;
            if (roots[j] < t)
                t = roots[j];
        }

    if (!intersected)
        return HitRecord();

    HitRecord record;
    auto p = ray.GetPoint(t);
    auto paramSq = a * a + b * b;
    auto distSq = p.lengthSquared();
    auto n = Vec3D(4 * p.x * (distSq - paramSq), 4 * p.y * (distSq - paramSq + 2 * a * a), 4 * p.z * (distSq - paramSq));
    record.Hit = true;
    record.Normal = (n * -ray.Direction > 0.0 ? n.normalised() : -n.normalised());
    record.NormalFlipped = n * ray.Direction > 0.0;
    record.HitPoint = p;
    record.MaterialPtr = materialPtr;
    record.Ray = ray;
    record.T = t;

    return record;
}
