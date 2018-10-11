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
	std::function<void(float)> successCallback;
	std::unique_ptr<ThreadPool> pool;
    CriticalSection criticalSection;
public:
	Renderer(std::function<void(void)> successCallback) : renderedCount(0), successCallback(successCallback)
	{
		pool.reset(new ThreadPool(16));
	}
    Renderer(const Renderer& other)
    {
        renderedCount = other.renderedCount;
        successCallback = other.successCallback;
        pool.reset(new ThreadPool(16));
    }
    Renderer(Renderer&& other)
    {
        renderedCount = std::move(other.renderedCount);
        successCallback = std::move(other.successCallback);
        pool = std::move(other.pool);
    }
    Renderer& operator=(const Renderer& other)
    {
        renderedCount = other.renderedCount;
        successCallback = other.successCallback;
        pool.reset(new ThreadPool(16));
        return *this;
    }
    Renderer& operator=(Renderer&& other)
    {
        renderedCount = std::move(other.renderedCount);
        successCallback = std::move(other.successCallback);
        pool = std::move(other.pool);
        return *this;
    }
	void Render(double& progress, const std::shared_ptr<Camera> camera, const std::shared_ptr<Tracer> tracer, const std::shared_ptr<ViewPlane> viewPlane)
	{
        
        Thread::launch([camera, tracer, viewPlane, threadDim = threadDim, pool = pool,
                        successCallback = successCallback,
                        &renderedCount = renderedCount,
                        &progress = progress,
                        &criticalSection = criticalSection]() {
        auto width = viewPlane->Width;
        auto height = viewPlane->Height;
        auto totalPixel = width * height;
        auto t0 = Time::getMillisecondCounterHiRes();
        for (int br = 0; br < ceil(height / threadDim); br++)
        {
            auto startR = br * threadDim, endR = std::min(height, (br + 1) * threadDim);
            for (int bc = 0; bc < ceil(width / threadDim); bc++)
            {
                auto startC = bc * threadDim, endC = std::min(width, (bc + 1) * threadDim);
                pool->addJob([camera, tracer, viewPlane, startR, endR, startC, endC, totalPixel,
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
        auto t1 = Time::getMillisecondCounterHiRes();
        DBG((t1 - t0) / 1000.0);
        successCallback();
        });
	}
};
