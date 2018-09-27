/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent   : public Component,
                        public ApplicationCommandTarget,
                        public MenuBarModel
{
public:
    enum CommandIDs
    {
        startRender,
        pauseRender,
        stopRender,
        settingWorld,
        settingLight,
        settingCamera,
        settingViewplane,
        settingMesh,
        settingMaterial
    };
    //==============================================================================
    MainComponent();
    ~MainComponent();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
    //==============================================================================
    // Your private member variables go here...
    ApplicationCommandManager commandManager;
    std::unique_ptr<MenuBarComponent> menuBar;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
