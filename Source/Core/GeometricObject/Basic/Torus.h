/*
  ==============================================================================

    Torus.h
    Created: 22 Nov 2018 12:49:17am
    Author:  bowen

  ==============================================================================
*/

#pragma once

#include "../GeometricObject.h"

class Torus : public GeometricObject
{
    FP_TYPE a, b;
protected:
    void UpdateBoundingBox() override;
public:
    Torus(FP_TYPE a, FP_TYPE b);
    virtual ~Torus() = default;

    virtual HitRecord Hit(const Ray& ray) override;

    static std::shared_ptr<GeometricObject> parse(StringArray& cmd, std::unordered_map<String, std::shared_ptr<void>>& env);
};
