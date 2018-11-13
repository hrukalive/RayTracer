/*
  ==============================================================================

    Texture.h
    Created: 13 Nov 2018 9:56:02am
    Author:  Bowen Sun

  ==============================================================================
*/

#pragma once

#include "../Utility.h"

class Texture
{
public:
    virtual ~Texture() = default;
    virtual RGBColor getColor(const HitRecord& record) const = 0;
};

class Checker3D : public Texture
{
    RGBColor color1, color2;
    FP_TYPE size;
public:
    virtual ~Checker3D() = default;
    void setSize(const FP_TYPE newSize)
    {
        size = newSize;
    }
    void setColor1(const RGBColor newColor1)
    {
        color1 = newColor1;
    }
    void setColor2(const RGBColor newColor2)
    {
        color2 = newColor2;
    }
};
