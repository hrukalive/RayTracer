/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    *progress = 0.5;
    
    menuBar.reset(new MenuBarComponent(this));
    image.reset(new ImageComponent());
    progressBar.reset(new ProgressBar(*progress));
    
    progressBar->setSize(0, 25);
    
    addAndMakeVisible(menuBar.get());
    addAndMakeVisible(image.get());
    addAndMakeVisible(progressBar.get());
    
#if JUCE_MAC
    MenuBarModel::setMacMainMenu(this);
    menuBar->setVisible(false);
#else
    menuBar->setVisible(true);
#endif
    progressBar->setVisible(true);
    
    setApplicationCommandManagerToWatch(&commandManager);
    commandManager.registerAllCommandsForTarget(this);
    addKeyListener(commandManager.getKeyMappings());
#if JUCE_MAC
    setSize(600, 400 + progressBar->getHeight());
#else
    setSize(600, 400 + progressBar->getHeight() + LookAndFeel::getDefaultLookAndFeel().getDefaultMenuBarHeight());
#endif
}

MainComponent::~MainComponent()
{
#if JUCE_MAC
    MenuBarModel::setMacMainMenu (nullptr);
#endif
}

//==============================================================================
StringArray MainComponent::getMenuBarNames()
{
    return { "Settings", "Render" };
}

PopupMenu MainComponent::getMenuForIndex (int menuIndex, const String& /*menuName*/)
{
    PopupMenu menu;
    
    if (menuIndex == 0)
    {
        menu.addCommandItem (&commandManager, CommandIDs::settingWorld);
        menu.addCommandItem (&commandManager, CommandIDs::settingViewplane);
        menu.addCommandItem (&commandManager, CommandIDs::settingCamera);
        menu.addCommandItem (&commandManager, CommandIDs::settingLight);
        menu.addCommandItem (&commandManager, CommandIDs::settingGeometry);
        menu.addCommandItem (&commandManager, CommandIDs::settingMaterial);
        menu.addCommandItem (&commandManager, CommandIDs::settingMesh);
    }
    else if (menuIndex == 1)
    {
        menu.addCommandItem (&commandManager, CommandIDs::startRender);
        menu.addCommandItem (&commandManager, CommandIDs::pauseRender);
        menu.addCommandItem (&commandManager, CommandIDs::stopRender);
    }
    
    return menu;
}

//==============================================================================
ApplicationCommandTarget* MainComponent::getNextCommandTarget()
{
    return findFirstTargetParentComponent();
}
void MainComponent::getAllCommands (Array<CommandID>& c)
{
    Array<CommandID> commands { CommandIDs::startRender,
        CommandIDs::pauseRender,
        CommandIDs::stopRender,
        CommandIDs::settingMesh,
        CommandIDs::settingMaterial,
        CommandIDs::settingLight,
        CommandIDs::settingCamera,
        CommandIDs::settingViewplane,
        CommandIDs::settingWorld,
        CommandIDs::settingGeometry };
    c.addArray (commands);
}

void MainComponent::getCommandInfo (CommandID commandID, ApplicationCommandInfo& result)
{
    switch (commandID)
    {
        case CommandIDs::startRender:
            result.setInfo ("Start", "Initiate the rendering process", "Render", 0);
#if JUCE_MAC
            result.addDefaultKeypress ('r', ModifierKeys::commandModifier);
#else
            result.addDefaultKeypress ('r', ModifierKeys::ctrlModifier);
#endif
            break;
        case CommandIDs::pauseRender:
            result.setInfo ("Pause", "Pause the rendering process", "Render", 0);
            break;
        case CommandIDs::stopRender:
            result.setInfo ("Stop", "Stop the rendering process", "Render", 0);
            break;
        case CommandIDs::settingGeometry:
            result.setInfo ("Geometry", "Geometry settings", "Settings", 0);
            break;
        case CommandIDs::settingMesh:
            result.setInfo ("Mesh", "Mesh settings", "Settings", 0);
            break;
        case CommandIDs::settingMaterial:
            result.setInfo ("Material", "Material settings", "Settings", 0);
            break;
        case CommandIDs::settingLight:
            result.setInfo ("Light", "Light settings", "Settings", 0);
            break;
        case CommandIDs::settingCamera:
            result.setInfo ("Camera", "Camera settings", "Settings", 0);
            break;
        case CommandIDs::settingViewplane:
            result.setInfo ("Viewplane", "Viewplane settings", "Settings", 0);
            break;
        case CommandIDs::settingWorld:
            result.setInfo ("World", "World settings", "Settings", 0);
            break;
        default:
            break;
    }
}

bool MainComponent::perform (const InvocationInfo& info)
{
    switch (info.commandID)
    {
        case CommandIDs::startRender:
            break;
        case CommandIDs::pauseRender:
            break;
        case CommandIDs::stopRender:
            break;
        case CommandIDs::settingGeometry:
            break;
        case CommandIDs::settingMesh:
            break;
        case CommandIDs::settingMaterial:
            break;
        case CommandIDs::settingLight:
            break;
        case CommandIDs::settingCamera:
            break;
        case CommandIDs::settingViewplane:
            break;
        case CommandIDs::settingWorld:
            break;
        default:
            return false;
    }
    
    return true;
}

//==============================================================================
void MainComponent::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    g.setFont (Font (16.0f));
    g.setColour (Colours::white);
    g.drawText ("Hello World!", getLocalBounds(), Justification::centred, true);
}

void MainComponent::resized()
{
    // This is called when the MainComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
    auto b = getLocalBounds();
#if !JUCE_MAC
    menuBar->setBounds (b.removeFromTop (LookAndFeel::getDefaultLookAndFeel().getDefaultMenuBarHeight()));
#endif
    image->setBounds(b.withHeight(400));
    progressBar->setBounds(b.removeFromBottom(progressBar->getHeight()));
}
