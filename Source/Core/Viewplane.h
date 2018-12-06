/*
  ==============================================================================

    Viewplane.h
    Created: 28 Sep 2018 10:59:33am
    Author:  Bowen Sun

  ==============================================================================
*/

#pragma once

#include "Utility/Utility.h"

class ViewPlane
{
public:
    bool isStereo = false;
    int Width = 512, Height = 512;
    FP_TYPE PixelSize = 1.0f;
	int NumPixelSamples = 4, NumLensSamples = 4;
    int NumAreaLightSamples = 4;
    int NumGlossySamples = 4;
    int NumPhoton = TOTAL_PHOTON;
    int NumNPhoton = N_PHOTON;
	std::shared_ptr<Image> RenderedImage = nullptr;
private:
	CriticalSection criticalSection;
	std::vector<std::vector<RGBColor>> renderedArray;
	std::vector<std::vector<int>> renderedCount;

    // For HDR conversion
    std::vector<FP_TYPE> logIntensityArray;

public:
    ViewPlane();
    ~ViewPlane() = default;

    inline Point2D GetPixelCenter(int x, int y)
    {
        return Point2D(PixelSize * (x - Width / 2.0), PixelSize * (Height / 2.0 - y));
    }

    void initialize();
    void Reset();
    void SetPixel(int x, int y, RGBColor radiance);

    static std::shared_ptr<ViewPlane> parse(StringArray& cmd, std::unordered_map<String, std::shared_ptr<void>>& env);

    // TODO: Fix this conversion
    void ShowHDR(FP_TYPE targetContrast = log10(5.0), FP_TYPE gamma = 1.0);
    void ShowClamped(FP_TYPE gamma = 1.0);
};
