/*
  ==============================================================================

    OBJParser.h
    Created: 4 Oct 2018 4:05:15pm
    Author:  Bowen Sun

  ==============================================================================
*/

#pragma once
#include "../../JuceLibraryCode/JuceHeader.h"
#include "GeometricObject/Mesh.h"

class OBJParser
{
public:
    Mesh parse(StringArray& content);
};
