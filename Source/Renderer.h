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
	Renderer(std::function<void(FP_TYPE)> successCallback) : renderedCount(0), successCallback(successCallback)
	{
		pool.setThreadPriorities(priority);
	}
    Renderer(const Renderer& other)
    {
        renderedCount = other.renderedCount;
        successCallback = other.successCallback;
		pool.setThreadPriorities(priority);
    }
    Renderer(Renderer&& other)
    {
        renderedCount = std::move(other.renderedCount);
        successCallback = std::move(other.successCallback);
		pool.setThreadPriorities(priority);
    }
    Renderer& operator=(const Renderer& other)
    {
        renderedCount = other.renderedCount;
        successCallback = other.successCallback;
		pool.setThreadPriorities(priority);
        return *this;
    }
    Renderer& operator=(Renderer&& other)
    {
        renderedCount = other.renderedCount;
        successCallback = std::move(other.successCallback);
		pool.setThreadPriorities(priority);
        return *this;
    }
	void Render(double& progress, std::shared_ptr<Camera> camera, std::shared_ptr<Tracer> tracer, std::shared_ptr<ViewPlane> viewPlane)
	{
		viewPlane->Reset();
		renderedCount = 0;
        Thread::launch([camera, tracer, viewPlane, threadDim = threadDim, successCallback = successCallback,
						&pool = pool,
                        &renderedCount = renderedCount,
                        &progress = progress,
                        &criticalSection = criticalSection]() {
			auto width = viewPlane->Width;
			auto height = viewPlane->Height;
			auto totalPixel = width * height;
			auto t0 = Time::getMillisecondCounterHiRes();
			for (int br = 0; br < (int)ceil((FP_TYPE)height / threadDim); br++)
			{
				auto startR = br * threadDim, endR = std::min(height, (br + 1) * threadDim);
				for (int bc = 0; bc < (int)ceil((FP_TYPE)width / threadDim); bc++)
				{
					auto startC = bc * threadDim, endC = std::min(width, (bc + 1) * threadDim);
					pool.addJob([camera, tracer, viewPlane, startR, endR, startC, endC, totalPixel,
								  &renderedCount = renderedCount,
								  &progress = progress,
								  &criticalSection = criticalSection]() {
						for (int r = startR; r < endR; r++)
						{
							for (int c = startC; c < endC; c++)
							{
								auto rays = camera->CreateRay(c, r);
								for (auto& ray : rays)
								{
									viewPlane->SetPixel(c, r, tracer->Trace(ray));
								}
								criticalSection.enter();
								renderedCount++;
								progress = renderedCount / double(totalPixel);
								criticalSection.exit();
							}
						}
					});
				}
			}
			while (pool.getNumJobs())
			{
				Thread::sleep(100);
			}
			DBG("Dispensing thread ends.");
			auto t1 = Time::getMillisecondCounterHiRes();
			successCallback((FP_TYPE)((t1 - t0) / 1000.0));
        });
	}
	void Cancel()
	{
		pool.removeAllJobs(true, 1000);
	}
};
