/*
  ==============================================================================

    IObject.h
    Created: 5 Oct 2018 7:38:12pm
    Author:  bowen

  ==============================================================================
*/

#pragma once
#include "../Utility.h"

class IObject
{
public:
    virtual ~IObject() {}
	virtual HitRecord Hit(const Ray& ray) = 0;
};
