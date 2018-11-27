/*
  ==============================================================================

    Material.cpp
    Created: 6 Oct 2018 4:38:22pm
    Author:  bowen

  ==============================================================================
*/

#include <algorithm>
#include "Material.h"
#include "../World.h"
#include "../Tracer/Tracer.h"
#include "../Globals.h"

RGBColor Material::GetLe(const HitRecord& record) { return BLACK; }
RGBColor Material::Shade(const HitRecord& record) { return BLACK; }
std::pair<bool, RGBColor> Material::PathShade(const HitRecord& record) { return std::make_pair(false, BLACK); }
RGBColor Material::PhotonShade(const HitRecord& record) { return BLACK; }
