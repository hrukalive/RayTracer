/*
  ==============================================================================

    Globals.cpp
    Created: 5 Nov 2018 11:24:25pm
    Author:  bowen

  ==============================================================================
*/

#include "Globals.h"

std::shared_ptr<World> world = nullptr;
std::shared_ptr<Tracer> tracer = nullptr;
std::shared_ptr<Sampler> sampler = nullptr;
std::shared_ptr<ViewPlane> viewPlane = nullptr;
std::shared_ptr<Camera> camera = nullptr;
PhotonMap* photonMap = nullptr;
BalancedPhotonMap* balancedPhotonMap = nullptr;
