#include "Camera.h"
#include "../Globals.h"

PinholeCamera::PinholeCamera(Point3D eye, Vec3D lookAt, Vec3D up, float dist) : Camera(eye, lookAt, up), dist(dist) {}
std::vector<Ray> PinholeCamera::CreateRay(int c, int r, FP_TYPE offset) const
{
    std::vector<Ray> ret;
    auto centerPos = viewPlane->GetPixelCenter(c, r);
    auto shifts = sampler->SampleSquare(viewPlane->NumPixelSamples);
    for (auto& shift : shifts)
    {
        auto tmp = centerPos + (shift * viewPlane->PixelSize);
        ret.push_back(Ray(eye, (u * (tmp.x + offset) + v * tmp.y - w * dist).normalised()));
    }
    return ret;
}
Ray PinholeCamera::CreateARay(int c, int r, FP_TYPE offset) const
{
    auto centerPos = viewPlane->GetPixelCenter(c, r);
    auto shift = sampler->SampleSquareSingle();
    auto tmp = centerPos + (shift * viewPlane->PixelSize);
    return Ray(eye, (u * (tmp.x + offset) + v * tmp.y - w * dist).normalised());
}
