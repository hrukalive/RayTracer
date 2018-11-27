/*
  ==============================================================================

    Globals.h
    Created: 5 Nov 2018 11:24:25pm
    Author:  bowen

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "World.h"
#include "Tracer/Tracer.h"
#include "Sampler/Sampler.h"
#include "Viewplane.h"
#include "Camera/Camera.h"
#include "Utility/photonmap.h"

extern std::shared_ptr<World> world;
extern std::shared_ptr<Tracer> tracer;
extern std::shared_ptr<Sampler> sampler;
extern std::shared_ptr<ViewPlane> viewPlane;
extern std::shared_ptr<Camera> camera;
extern CriticalSection photonMapLock;
extern PhotonMap* photonMap;
extern BalancedPhotonMap* balancedPhotonMap;
