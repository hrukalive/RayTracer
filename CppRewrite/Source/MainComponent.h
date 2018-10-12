/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "Core/Utility.h"
#include "Core/Materials/BRDF.h"
#include "Core/Materials/Material.h"
#include "Core/World.h"
#include "Core/Sampler.h"
#include "Core/Tracer.h"
#include "Core/Camera.h"
#include "Core/Viewplane.h"
#include "Core/OBJParser.h"
#include "Renderer.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent   : public Component,
                        public ApplicationCommandTarget,
                        public MenuBarModel,
						public Timer
{
public:
    enum CommandIDs
    {
        startRender = 1,
        stopRender,
		saveRender,
        settingWorld,
        settingLight,
        settingCamera,
        settingViewplane,
        settingMesh,
        settingGeometry,
        settingMaterial
    };
    //==============================================================================
    MainComponent();
    ~MainComponent();

    //==============================================================================
    StringArray getMenuBarNames() override;
    PopupMenu getMenuForIndex (int menuIndex, const String& /*menuName*/) override;
    void menuItemSelected (int /*menuItemID*/, int /*topLevelMenuIndex*/) override {};
    
    //==============================================================================
    ApplicationCommandTarget* getNextCommandTarget() override;
    void getAllCommands (Array<CommandID>& c) override;
    void getCommandInfo (CommandID commandID, ApplicationCommandInfo& result) override;
    bool perform (const InvocationInfo& info) override;
    
    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
	void renderSucceeded(double timeElapsed);
	void timerCallback() override;
    //==============================================================================
    // Your private member variables go here...
    ApplicationCommandManager commandManager;
    std::unique_ptr<MenuBarComponent> menuBar;
    std::unique_ptr<ImageComponent> image;
    std::unique_ptr<ProgressBar> progressBar;
    double progress = -1.0;
	size_t timerSkip = 0;
	bool rendering = false;
	bool renderFinished = false;
    
    std::shared_ptr<World> world;
    std::shared_ptr<Tracer> tracer;
    std::shared_ptr<Sampler> sampler;
    std::shared_ptr<ViewPlane> viewPlane;
    std::shared_ptr<Camera> camera;
    Renderer renderer{Renderer(std::bind(&MainComponent::renderSucceeded, this, std::placeholders::_1))};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
