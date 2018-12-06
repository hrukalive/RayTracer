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
    auto realWidth = isStereo ? 2 * Width : Width;

    renderedArray.resize(Height);
    for (auto& row : renderedArray)
        row.resize(realWidth);
    renderedCount.resize(Height);
    for (auto& row : renderedCount)
        row.resize(realWidth);

    logIntensityArray.resize(Height * realWidth);

    RenderedImage = std::make_shared<Image>(Image(Image::PixelFormat::RGB, realWidth, Height, true));
    for (int i = 0; i < realWidth; i++) {
        for (int j = 0; j < Height; j++)
        {
            RenderedImage->setPixelAt(i, j, Colour::fromFloatRGBA(0.0, 0.0, 0.0, 1.0));
        }
    }
}

ViewPlane::ViewPlane() {}

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

std::shared_ptr<ViewPlane> ViewPlane::parse(StringArray& cmd, std::unordered_map<String, std::shared_ptr<void>>& env)
{
    std::shared_ptr<ViewPlane> ret = nullptr;
    ret.reset(new ViewPlane());
    ret->Width = std::stoi(cmd[1].toStdString());
    ret->Height = std::stoi(cmd[2].toStdString());
    ret->PixelSize = std::stod(cmd[3].toStdString());
    ret->NumPixelSamples = std::stoi(cmd[4].toStdString());
    ret->NumLensSamples = std::stoi(cmd[5].toStdString());
    ret->NumAreaLightSamples = std::stoi(cmd[6].toStdString());
    ret->NumGlossySamples = std::stoi(cmd[7].toStdString());
    ret->NumPhoton = std::stoi(cmd[8].toStdString());
    ret->NumNPhoton = std::stoi(cmd[9].toStdString());
    ret->isStereo = cmd[10] == "true";
    ret->initialize();
    return ret;
}
