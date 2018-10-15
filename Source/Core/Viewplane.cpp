/*
  ==============================================================================

    Viewplane.cpp
    Created: 28 Sep 2018 10:59:33am
    Author:  Bowen Sun

  ==============================================================================
*/

#include "Viewplane.h"

void ViewPlane::initialize()
{
    SqrtNumSamplePixel = (int)sqrt(NumPixelSamples);
    SqrtNumSampleLens = (int)sqrt(NumLensSamples);

    renderedArray.resize(Height);
    for (auto& row : renderedArray)
        row.resize(Width);
    renderedCount.resize(Height);
    for (auto& row : renderedCount)
        row.resize(Width);

    logIntensityArray.resize(Height * Width);

    RenderedImage = std::make_shared<Image>(Image(Image::PixelFormat::RGB, Width, Height, true));
    for (int i = 0; i < Width; i++) {
        for (int j = 0; j < Height; j++)
        {
            RenderedImage->setPixelAt(i, j, Colour::fromFloatRGBA(0.0, 0.0, 0.0, 1.0));
        }
    }
}

ViewPlane::ViewPlane() { initialize(); }
ViewPlane::ViewPlane(int width, int height, FP_TYPE pixelSize)
    : Width(width), Height(height), PixelSize(pixelSize)
{
    initialize();
}
ViewPlane::ViewPlane(int width, int height, FP_TYPE pixelSize, int numSamplePixels)
    : Width(width), Height(height), PixelSize(pixelSize), NumPixelSamples(numSamplePixels)
{
    initialize();
}
ViewPlane::ViewPlane(int width, int height, FP_TYPE pixelSize, int numSamplePixels, int numSampleLens)
    : Width(width), Height(height), PixelSize(pixelSize), NumPixelSamples(numSamplePixels), NumLensSamples(numSampleLens)
{
    initialize();
}

void ViewPlane::Reset()
{
    renderedArray.resize(0);
    renderedCount.resize(0);
    logIntensityArray.resize(0);
    initialize();
}

void ViewPlane::SetPixel(int x, int y, RGBColor radiance)
{
    criticalSection.enter();
    renderedArray[y][x] += radiance;
    renderedCount[y][x]++;
    auto color = renderedArray[y][x] / renderedCount[y][x];
    RenderedImage->setPixelAt(x, y, Colour::fromFloatRGBA((float)color.x, (float)color.y, (float)color.z, 1.0));
    logIntensityArray[y * Width + x] = log10(1.0 / 61.0 * (20.0 * color.x + 40.0 * color.y + color.z) + 1e-7);
    criticalSection.exit();
}

// TODO: Fix this conversion
void ViewPlane::ShowHDR(FP_TYPE targetContrast, FP_TYPE gamma)
{
    criticalSection.enter();
    auto logBase = bilaterialFilter(logIntensityArray, Width, Height);
    auto logBaseMin = *std::min_element(logBase.begin(), logBase.end());
    auto logBaseMax = *std::max_element(logBase.begin(), logBase.end());
    auto compressionFactor = targetContrast / (logBaseMax - logBaseMin);
    auto logAbsScale = logBaseMax * compressionFactor;

    std::vector<FP_TYPE> logOutputI;
    logOutputI.resize(Height * Width);
    for (auto i = 0; i < logIntensityArray.size(); i++)
        logOutputI[i] = pow(10, logBase[i] * compressionFactor + (logIntensityArray[i] - logBase[i]) - logAbsScale);

    for (int j = 0; j < Height; j++)
    {
        for (int i = 0; i < Width; i++)
        {
            auto color = renderedArray[j][i] / renderedCount[j][i];
            auto intensity = 1.0 / 61.0 * (20.0 * color.x + 40.0 * color.y + color.z);
            auto outputLogI = logOutputI[j * Width + i];
            RenderedImage->setPixelAt(i, j, Colour::fromFloatRGBA(
                (float)pow(color.x / intensity * outputLogI, gamma),
                (float)pow(color.y / intensity * outputLogI, gamma),
                (float)pow(color.z / intensity * outputLogI, gamma), 1.0));
        }
    }
    criticalSection.exit();
}
void ViewPlane::ShowClamped(FP_TYPE gamma)
{
    criticalSection.enter();
    for (int j = 0; j < Height; j++)
    {
        for (int i = 0; i < Width; i++)
        {
            auto color = renderedArray[j][i] / renderedCount[j][i];
            RenderedImage->setPixelAt(i, j, Colour::fromFloatRGBA(
                (float)pow(color.x, gamma),
                (float)pow(color.y, gamma),
                (float)pow(color.z, gamma), 1.0));
        }
    }
    criticalSection.exit();
}
