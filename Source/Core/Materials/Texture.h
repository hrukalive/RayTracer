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
