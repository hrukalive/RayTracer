/*
  ==============================================================================

    Globals.h
    Created: 5 Nov 2018 11:24:25pm
    Author:  bowen

  ==============================================================================
*/

#pragma once

#include "World.h"
#include "Tracer.h"
#include "Sampler.h"
#include "Viewplane.h"
#include "Camera.h"

extern std::shared_ptr<World> world;
extern std::shared_ptr<Tracer> tracer;
extern std::shared_ptr<Sampler> sampler;
extern std::shared_ptr<ViewPlane> viewPlane;
extern std::shared_ptr<Camera> camera;
