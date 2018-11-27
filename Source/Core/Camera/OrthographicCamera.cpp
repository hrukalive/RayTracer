#include "Camera.h"
#include "../Globals.h"

OrthographicCamera::OrthographicCamera(Point3D eye, Vec3D lookAt, Vec3D up) : Camera(eye, lookAt, up) {}
std::vector<Ray> OrthographicCamera::CreateRay(int c, int r, FP_TYPE offset) const
{
    std::vector<Ray> ret;
    auto centerPos = viewPlane->GetPixelCenter(c, r);
    auto shifts = sampler->SampleSquare(viewPlane->NumPixelSamples);
    for (auto& shift : shifts)
    {
        auto tmp = centerPos + shift * viewPlane->PixelSize;
        ret.push_back(Ray(u * (tmp.x + offset) + v * tmp.y + eye, -w));
    }
    return ret;
}
Ray OrthographicCamera::CreateARay(int c, int r, FP_TYPE offset) const
{
    auto centerPos = viewPlane->GetPixelCenter(c, r);
    auto shift = sampler->SampleSquareSingle();
    auto tmp = centerPos + shift * viewPlane->PixelSize;
    return Ray(u * (tmp.x + offset) + v * tmp.y + eye, -w);
}
