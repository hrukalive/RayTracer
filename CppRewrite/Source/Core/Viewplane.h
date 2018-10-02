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
    int Width = 512, Height = 512;
    float PixelSize = 1.0f;
	int NumPixelSamples = 64, NumLensSamples = 1;
	int SqrtNumSamplePixel, SqrtNumSampleLens;
	float Gamma = 1.0f;
	std::shared_ptr<Image> RenderedImage;
private:
	CriticalSection criticalSection;
	std::vector<std::vector<RGBColor>> renderedArray;
	std::vector<std::vector<int>> renderedCount;

	inline void initialize()
	{
		SqrtNumSamplePixel = (int)sqrt(NumPixelSamples);
		SqrtNumSampleLens = (int)sqrt(NumLensSamples);

		renderedArray.resize(Height);
		for (auto& row : renderedArray)
			row.resize(Width);
		renderedCount.resize(Height);
		for (auto& row : renderedCount)
			row.resize(Width);

		RenderedImage = std::make_shared<Image>(Image(Image::PixelFormat::RGB, Width, Height, true));
	}
public:
	ViewPlane() { initialize(); }
	ViewPlane(int width, int height, float pixelSize) 
		: Width(width), Height(height), PixelSize(pixelSize)
	{
		initialize();
	}
	ViewPlane(int width, int height, float pixelSize, int numSamplePixels, int numSampleLens)
		: Width(width), Height(height), PixelSize(pixelSize), NumPixelSamples(numSamplePixels), NumLensSamples(numSampleLens)
	{
		initialize();
	}

	inline Point2D GetPixelCenter(int x, int y)
	{
		return Point2D(PixelSize * (x - Width / 2.0), PixelSize * (Height / 2.0 - y));
	}

	void Reset()
	{
		renderedArray.resize(0);
		renderedCount.resize(0);
		initialize();
	}

	void SetPixel(int x, int y, RGBColor radiance)
	{
		criticalSection.enter();
		renderedArray[y][x] += radiance;
		renderedCount[y][x]++;
		auto color = renderedArray[y][x] / renderedCount[y][x];
		RenderedImage->setPixelAt(x, y, 
			Colour::fromFloatRGBA((float)pow(color.x, Gamma), (float)pow(color.y, Gamma), (float)pow(color.z, Gamma), 1.0));
		criticalSection.exit();
	}
};
