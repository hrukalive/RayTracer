/*
  ==============================================================================

    Renderer.cpp
    Created: 10 Oct 2018 8:53:12pm
    Author:  bowen

  ==============================================================================
*/

#include <algorithm>
#include "Renderer.h"

Renderer::Renderer(std::function<void(FP_TYPE)> successCallback) : renderedCount(0), successCallback(successCallback)
{
    pool.setThreadPriorities(priority);
}
Renderer::Renderer(const Renderer& other)
{
    renderedCount = other.renderedCount;
    successCallback = other.successCallback;
    pool.setThreadPriorities(priority);
}
Renderer::Renderer(Renderer&& other)
{
    renderedCount = std::move(other.renderedCount);
    successCallback = std::move(other.successCallback);
    pool.setThreadPriorities(priority);
}
Renderer& Renderer::operator=(const Renderer& other)
{
    renderedCount = other.renderedCount;
    successCallback = other.successCallback;
    pool.setThreadPriorities(priority);
    return *this;
}
Renderer& Renderer::operator=(Renderer&& other)
{
    renderedCount = other.renderedCount;
    successCallback = std::move(other.successCallback);
    pool.setThreadPriorities(priority);
    return *this;
}

void Renderer::Render(double& progress, std::shared_ptr<Camera> camera, std::shared_ptr<Tracer> tracer, std::shared_ptr<ViewPlane> viewPlane)
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
        std::vector<std::pair<int, int>> blocks;
        for (int i = 0; i < (int)ceil((FP_TYPE)height / threadDim); i++)
            for (int j = 0; j < (int)ceil((FP_TYPE)width / threadDim); j++)
                blocks.push_back(std::make_pair(i, j));
        std::random_shuffle(blocks.begin(), blocks.end());
        auto t0 = Time::getMillisecondCounterHiRes();
        for (auto& block : blocks)
        {
            int br = block.first;
            int bc = block.second;
            auto startR = br * threadDim, endR = std::min(height, (br + 1) * threadDim);
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
                            viewPlane->SetPixel(c, r, tracer->Trace(ray));
                        criticalSection.enter();
                        renderedCount++;
                        progress = renderedCount / double(totalPixel);
                        criticalSection.exit();
                    }
                }
                DBG(progress * 100);
            });
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
bool Renderer::Cancel()
{
    pool.removeAllJobs(true, 1000);
    return pool.getNumJobs() == 0;
}
