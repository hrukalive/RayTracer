/*
  ==============================================================================

    Viewplane.h
    Created: 28 Sep 2018 10:59:33am
    Author:  Bowen Sun

  ==============================================================================
*/

#pragma once

#include "Utility.h"

class ViewPlane
{
public:
    bool isStereo;
    int Width = 512, Height = 512;
    FP_TYPE PixelSize = 1.0f;
	int NumPixelSamples = 64, NumLensSamples = 1;
    int NumAreaLightSamples = 4;
    int NumGlossySamples = 4;
	int SqrtNumSamplePixel, SqrtNumSampleLens;
	std::shared_ptr<Image> RenderedImage;
private:
	CriticalSection criticalSection;
	std::vector<std::vector<RGBColor>> renderedArray;
	std::vector<std::vector<int>> renderedCount;

    // For HDR conversion
    std::vector<FP_TYPE> logIntensityArray;

    inline void initialize();
public:
    ViewPlane(bool isStereo = false);
    ViewPlane(int width, int height, FP_TYPE pixelSize, bool isStereo = false);
    ViewPlane(int width, int height, FP_TYPE pixelSize, int numSamplePixels, bool isStereo = false);
    ViewPlane(int width, int height, FP_TYPE pixelSize, int numSamplePixels, int numSampleLens, bool isStereo = false);

    inline Point2D GetPixelCenter(int x, int y)
    {
        return Point2D(PixelSize * (x - Width / 2.0), PixelSize * (Height / 2.0 - y));
    }

    void Reset();
    void SetPixel(int x, int y, RGBColor radiance);

    // TODO: Fix this conversion
    void ShowHDR(FP_TYPE targetContrast = log10(5.0), FP_TYPE gamma = 1.0);
    void ShowClamped(FP_TYPE gamma = 1.0);
};
