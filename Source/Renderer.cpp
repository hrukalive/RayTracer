/*
  ==============================================================================

    Renderer.cpp
    Created: 10 Oct 2018 8:53:12pm
    Author:  bowen

  ==============================================================================
*/

#include <algorithm>
#include "Renderer.h"
#include "Core/Globals.h"

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

void Renderer::Render(double& progress)
{
    viewPlane->Reset();
    renderedCount = 0;
    Thread::launch([threadDim = threadDim, successCallback = successCallback,
        &pool = pool,
        &renderedCount = renderedCount,
        &progress = progress,
        &criticalSection = criticalSection]() {

        auto width = viewPlane->isStereo ? 2 * viewPlane->Width : viewPlane->Width;
        auto height = viewPlane->Height;
        std::vector<std::pair<int, int>> blocks;
        for (int i = 0; i < (int)ceil((FP_TYPE)height / threadDim); i++)
            for (int j = 0; j < (int)ceil((FP_TYPE)width / threadDim); j++)
                blocks.push_back(std::make_pair(i, j));
        std::random_shuffle(blocks.begin(), blocks.end());
        auto t0 = Time::getMillisecondCounterHiRes();

        if (dynamic_cast<const PhotonMapTrace*>(tracer.get()) != nullptr)
        {
            auto tracerBackup = tracer;
            tracer.reset(new PhotonMapPreTrace(20));
            for (auto& l : world->GetLights())
            {
                auto photons = l->EmitPhoton();
                auto totalPixel = (int)photons.size();
                auto renderCount = 0;
                for (int block = 0; block < (int)ceil((FP_TYPE)photons.size() / (threadDim * threadDim)); block++)
                {
                    auto start = block * (threadDim * threadDim), end = std::min((int)photons.size(), (block + 1) * (threadDim * threadDim));
                    pool.addJob([start, end, totalPixel,
                        &rays = photons,
                        &renderedCount = renderedCount,
                        &progress = progress,
                        &criticalSection = criticalSection]() {
                        for (int r = start; r < end; r++)
                        {
                            tracer->Trace(rays[r]);
                            criticalSection.enter();
                            renderedCount++;
                            progress = renderedCount / double(totalPixel);
                            criticalSection.exit();
                        }
                    });
                }

                while (pool.getNumJobs())
                {
                    Thread::sleep(100);
                }
                scalePhotonPower(photonMap, 1.0 / photons.size());
                renderedCount = 0;
                progress = 0;
            }
            DBG("Photon tracing done.");
            tracer = tracerBackup;
            progress = -1;
            balancedPhotonMap = balancePhotonMap(photonMap);
        }
        progress = 0;
        if (dynamic_cast<const PathTrace*>(tracer.get()) != nullptr || dynamic_cast<const PhotonMapTrace*>(tracer.get()) != nullptr)
        {
            auto totalPixel = width * height * viewPlane->NumPixelSamples;
            for (int iter = 0; iter < viewPlane->NumPixelSamples; iter++)
            {
                for (auto& block : blocks)
                {
                    int br = block.first;
                    int bc = block.second;
                    auto startR = br * threadDim, endR = std::min(height, (br + 1) * threadDim);
                    auto startC = bc * threadDim, endC = std::min(width, (bc + 1) * threadDim);
                    pool.addJob([startR, endR, startC, endC, totalPixel,
                        &renderedCount = renderedCount,
                        &progress = progress,
                        &criticalSection = criticalSection]() {
                        for (int r = startR; r < endR; r++)
                        {
                            for (int c = startC; c < endC; c++)
                            {
                                auto ray = camera->CreateARay(c, r);
                                auto traceResult = tracer->Trace(ray);
                                if (traceResult.first)
                                    viewPlane->SetPixel(c, r, traceResult.second);
                                criticalSection.enter();
                                renderedCount++;
                                progress = renderedCount / double(totalPixel);
                                criticalSection.exit();
                            }
                        }
                    });
                }
            }
        }
        else
        {
            auto totalPixel = width * height;
            for (auto& block : blocks)
            {
                int br = block.first;
                int bc = block.second;
                auto startR = br * threadDim, endR = std::min(height, (br + 1) * threadDim);
                auto startC = bc * threadDim, endC = std::min(width, (bc + 1) * threadDim);
                pool.addJob([startR, endR, startC, endC, totalPixel,
                    &renderedCount = renderedCount,
                    &progress = progress,
                    &criticalSection = criticalSection]() {
                    for (int r = startR; r < endR; r++)
                    {
                        for (int c = startC; c < endC; c++)
                        {
                            auto rays = camera->CreateRay(c, r);
                            for (auto& ray : rays)
                                viewPlane->SetPixel(c, r, tracer->Trace(ray).second);
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
bool Renderer::Cancel()
{
    pool.removeAllJobs(true, 1000);
    return pool.getNumJobs() == 0;
}
