/*
  ==============================================================================

    Renderer.h
    Created: 10 Oct 2018 8:53:12pm
    Author:  bowen

  ==============================================================================
*/

#pragma once

#include "Core/Utility.h"
#include "Core/World.h"
#include "Core/Viewplane.h"
#include "Core/Camera.h"
#include "Core/Tracer.h"

class Renderer
{
	size_t renderedCount;
    int threadDim = 8;
	int priority = 3;
	std::function<void(FP_TYPE)> successCallback;
	ThreadPool pool;
    CriticalSection criticalSection;
public:
    Renderer(std::function<void(FP_TYPE)> successCallback);
    Renderer(const Renderer& other);
    Renderer(Renderer&& other);
    Renderer& operator=(const Renderer& other);
    Renderer& operator=(Renderer&& other);

    void Render(double& progress, std::shared_ptr<Camera> camera, std::shared_ptr<Tracer> tracer, std::shared_ptr<ViewPlane> viewPlane);
    bool Cancel();
};
