#include "Camera.h"
#include "../Globals.h"

ThinLensCamera::ThinLensCamera(Point3D eye, Vec3D lookAt, Vec3D up, float dist, float focalDist, float lensRadius)
    : Camera(eye, lookAt, up), dist(dist), f(focalDist), radius(lensRadius) {}

std::vector<Ray> ThinLensCamera::CreateRay(int c, int r, FP_TYPE offset) const
{
    std::vector<Ray> ret;
    auto centerPos = viewPlane->GetPixelCenter(c, r);
    auto shifts = sampler->SampleSquare(viewPlane->NumPixelSamples);
    for (auto& shift : shifts)
    {
        auto tmp = centerPos + (shift * viewPlane->PixelSize);
        tmp.x = tmp.x * f / dist;
        tmp.y = tmp.y * f / dist;
        
        auto lensShifts = sampler->SampleCircle(viewPlane->NumLensSamples);
        for (auto& lensShift : lensShifts)
        {
            ret.push_back(Ray(eye + u * (lensShift.x * radius) + v * (lensShift.y * radius), 
                (u * (tmp.x - lensShift.x * radius + offset) + v * (tmp.y - lensShift.y * radius) - w * f).normalised()));
        }
    }
    return ret;
}

Ray ThinLensCamera::CreateARay(int c, int r, FP_TYPE offset) const
{
    auto centerPos = viewPlane->GetPixelCenter(c, r);
    auto shift = sampler->SampleSquareSingle();
    auto tmp = centerPos + (shift * viewPlane->PixelSize);
    tmp.x = tmp.x * f / dist;
    tmp.y = tmp.y * f / dist;

    auto lensShift = sampler->SampleCircleSingle();
    return Ray(eye + u * (lensShift.x * radius) + v * (lensShift.y * radius), 
        (u * (tmp.x - lensShift.x * radius + offset) + v * (tmp.y - lensShift.y * radius) - w * f).normalised());
}
