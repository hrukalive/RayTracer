#include "Texture.h"

void ConstTexture::setColor(RGBColor color)
{
    c = color;
}

RGBColor ConstTexture::getColor(const HitRecord& record) const
{
    return c;
}
