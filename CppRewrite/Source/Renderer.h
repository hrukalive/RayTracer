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
	double progress;
	std::function<void(void)> successCallback;
	std::unique_ptr<ThreadPool> pool;
public:
	Renderer(std::function<void(void)> successCallback) : progress(0.0), successCallback(successCallback)
	{
		pool.reset(new ThreadPool(16));
	}
	double GetProgress() { return progress; }
	void Render(const std::shared_ptr<Camera> camera, const std::shared_ptr<Tracer> tracer, const std::shared_ptr<ViewPlane> viewPlane)
	{
		pool->addJob([=]() {
			auto t0 = Time::getMillisecondCounterHiRes();
			for (int r = 0; r < viewPlane->Height; r++)
			{
				for (int c = 0; c < viewPlane->Width; c++)
				{
					auto rays = camera->CreateRay(c, r);
					for (auto& ray : rays)
					{
						viewPlane->SetPixel(c, r, tracer->Trace(ray));
					}
				}
				DBG(r);
			}
			auto t1 = Time::getMillisecondCounterHiRes();
			DBG((t1 - t0) / 1000.0);
			successCallback();
		});
	}
};
