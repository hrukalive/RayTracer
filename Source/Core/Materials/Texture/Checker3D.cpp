#include "Texture.h"

void Checker3D::setSize(const FP_TYPE newSize)
{
    size = newSize;
}

void Checker3D::setColor1(const RGBColor newColor1)
{
    color1 = newColor1;
}

void Checker3D::setColor2(const RGBColor newColor2)
{
    color2 = newColor2;
}

RGBColor Checker3D::getColor(const HitRecord& record) const
{
    FP_TYPE x = record.HitPoint.x - kEpsilon, y = record.HitPoint.y - kEpsilon, z = record.HitPoint.z - kEpsilon;
    if (((int)floor(x / size) + (int)floor(y / size) + (int)floor(z / size)) % 2 == 0)
        return color1;
    else
        return color2;
}
