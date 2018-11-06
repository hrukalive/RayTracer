/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    world.reset(new World());
    tracer.reset(new Whitted(world));
    viewPlane.reset(new ViewPlane(vpWidth, vpHeight, (FP_TYPE)(1.0 / vpHeight), 4, 4));
    sampler.reset(new MultiJittered());
    
    setupWorld();

    menuBar.reset(new MenuBarComponent(this));
    image.reset(new ImageComponent());
    progressBar.reset(new ProgressBar(progress));
    
    
    addAndMakeVisible(menuBar.get());
    addAndMakeVisible(image.get());
    addAndMakeVisible(progressBar.get());
    
#if JUCE_MAC
    MenuBarModel::setMacMainMenu(this);
    menuBar->setVisible(false);
#else
    menuBar->setVisible(true);
#endif
	progressBar->setSize(0, 0);
    progressBar->setVisible(false);
    
    setApplicationCommandManagerToWatch(&commandManager);
    commandManager.registerAllCommandsForTarget(this);
    addKeyListener(commandManager.getKeyMappings());
#if JUCE_MAC
    setSize(std::max(100, vpWidth), std::max(100, vpHeight));
#else
    setSize(std::max(100, vpWidth), std::max(100, vpHeight) + LookAndFeel::getDefaultLookAndFeel().getDefaultMenuBarHeight());
#endif
	startTimer(1000);
}

MainComponent::~MainComponent()
{
    while (!renderer.Cancel()) {}

    world = nullptr;
    tracer = nullptr;
    sampler = nullptr;
    viewPlane = nullptr;
    camera = nullptr;

#if JUCE_MAC
    MenuBarModel::setMacMainMenu (nullptr);
#endif
}

void MainComponent::setupWorld()
{
    auto r = 2.7;
    auto theta = 145.0 * PI_OVER_180;
    auto phi = 70 * PI_OVER_180;
    auto roll = 0.0 * PI_OVER_180;
    auto lookat = Vec3D(0.2, 0.7, 0.0);
    auto eyepoint = Vec3D(r * sin(theta) * sin(phi), r * cos(phi), r * cos(theta) * sin(phi)) + lookat;
    camera.reset(new PinholeCamera(eyepoint, lookat, Vec3D(sin(roll), cos(roll), 0.0), 1.0, viewPlane, sampler));
    //camera.reset(new ThinLensCamera(eyepoint, lookat, Vec3D(sin(roll), cos(roll), 0.0), 1.0, 18000.0, 100.0, viewPlane, sampler));

    std::shared_ptr<Light> ambient{ new Ambient(0.2, RGBColor(1.0, 1.0, 1.0)) };
    world->SetAmbient(ambient);
    
    std::shared_ptr<GeometricObject> lightplane1{ new RayTracer::Rectangle(Point3D(-0.3, 0.5, 1.5), Vec3D(0, 0.5, 0), Vec3D(0.6, 0, 0)) };
    std::shared_ptr<Material> lightMat1{ new Emissive() };
    std::dynamic_pointer_cast<Emissive>(lightMat1)->SetLs(30);
    std::dynamic_pointer_cast<Emissive>(lightMat1)->SetCe(RGBColor(1.0, 1.0, 1.0));
    lightplane1->SetMaterial(lightMat1);
    std::shared_ptr<Light> arealight1{ new AreaLight(lightplane1, lightMat1) };
    world->AddLight(arealight1);
    
    std::shared_ptr<GeometricObject> lightplane2{ new RayTracer::Rectangle(Point3D(1.4, 0.9, 0.0), Vec3D(0, 0, 0.6), Vec3D(0, 0.5, 0)) };
    std::shared_ptr<Material> lightMat2{ new Emissive() };
    std::dynamic_pointer_cast<Emissive>(lightMat2)->SetLs(15);
    std::dynamic_pointer_cast<Emissive>(lightMat2)->SetCe(RGBColor(1.0, 1.0, 0.5));
    lightplane2->SetMaterial(lightMat2);
    std::shared_ptr<Light> arealight2{ new AreaLight(lightplane2, lightMat2) };
    world->AddLight(arealight2);
    
    std::shared_ptr<GeometricObject> lightplane3{ new RayTracer::Rectangle(Point3D(-1.4, 0.9, 0.0), Vec3D(0, 0.5, 0), Vec3D(0, 0, 0.6)) };
    std::shared_ptr<Material> lightMat3{ new Emissive() };
    std::dynamic_pointer_cast<Emissive>(lightMat3)->SetLs(15);
    std::dynamic_pointer_cast<Emissive>(lightMat3)->SetCe(RGBColor(0.5, 1.0, 1.0));
    lightplane3->SetMaterial(lightMat3);
    std::shared_ptr<Light> arealight3{ new AreaLight(lightplane3, lightMat3) };
    world->AddLight(arealight3);

    std::shared_ptr<GeometricObject> plane{ new RayTracer::Rectangle(Point3D(-3, 0, 3), Vec3D(6, 0, 0), Vec3D(0, 0, -6)) };
    std::shared_ptr<Material> planeMat{ new Matte() };
    std::dynamic_pointer_cast<Matte>(planeMat)->SetKa(0.3);
    std::dynamic_pointer_cast<Matte>(planeMat)->SetKd(0.7);
    std::dynamic_pointer_cast<Matte>(planeMat)->SetCd(RGBColor(1.0, 1.0, 1.0));
    plane->SetMaterial(planeMat);

    std::shared_ptr<GeometricObject> box1{ new Box(Point3D(-1, 0, 0), Point3D(-0.7, 1, 0.3)) };
    
    std::shared_ptr<GeometricObject> box2{ new Box(Point3D(-0.43, 0, 0.2), Point3D(-0.13, 1, 0.5)) };
    std::shared_ptr<Material> boxMat2{ new Reflective() };
    std::dynamic_pointer_cast<Phong>(boxMat2)->SetKa(0.3);
    std::dynamic_pointer_cast<Phong>(boxMat2)->SetKd(0.3);
    std::dynamic_pointer_cast<Phong>(boxMat2)->SetKs(0.3);
    std::dynamic_pointer_cast<Phong>(boxMat2)->SetE(80.0);
    std::dynamic_pointer_cast<Phong>(boxMat2)->SetCd(RGBColor(0.0, 1.0, 0.8));
    std::dynamic_pointer_cast<Phong>(boxMat2)->SetCs(RGBColor(1.0, 1.0, 1.0));
    std::dynamic_pointer_cast<Reflective>(boxMat2)->SetCr(RGBColor(1.0, 1.0, 1.0));
    std::dynamic_pointer_cast<Reflective>(boxMat2)->SetKr(0.5);
    box2->SetMaterial(boxMat2);
    
    std::shared_ptr<GeometricObject> box3{ new Box(Point3D(0.13, 0, 0), Point3D(0.43, 1, 0.3)) };
    std::shared_ptr<GeometricObject> box4{ new Box(Point3D(0.7, 0, 0.2), Point3D(1, 1, 0.5)) };
    std::shared_ptr<Material> boxMat{ new Phong() };
    std::dynamic_pointer_cast<Phong>(boxMat)->SetKa(0.3);
    std::dynamic_pointer_cast<Phong>(boxMat)->SetKd(0.3);
    std::dynamic_pointer_cast<Phong>(boxMat)->SetKs(0.3);
    std::dynamic_pointer_cast<Phong>(boxMat)->SetE(80.0);
    std::dynamic_pointer_cast<Phong>(boxMat)->SetCd(RGBColor(0.0, 1.0, 0.8));
    std::dynamic_pointer_cast<Phong>(boxMat)->SetCs(RGBColor(1.0, 1.0, 1.0));
    box1->SetMaterial(boxMat);
    box3->SetMaterial(boxMat);
    box4->SetMaterial(boxMat);
    
    std::shared_ptr<GeometricObject> sphere1{ new Sphere(Point3D(-0.3, 0.3, -0.6), 0.3) };
    std::shared_ptr<Material> sphereMat1{ new Reflective() };
    std::dynamic_pointer_cast<Phong>(sphereMat1)->SetKa(0.0);
    std::dynamic_pointer_cast<Phong>(sphereMat1)->SetKd(0.3);
    std::dynamic_pointer_cast<Phong>(sphereMat1)->SetKs(0.0);
    std::dynamic_pointer_cast<Reflective>(sphereMat1)->SetCr(RGBColor(1.0, 0.7, 0.7));
    std::dynamic_pointer_cast<Reflective>(sphereMat1)->SetKr(0.8);
    sphere1->SetMaterial(sphereMat1);
    
    std::shared_ptr<GeometricObject> sphere2{ new Sphere(Point3D(0.6, 0.3, -0.6), 0.3) };
    std::shared_ptr<Material> sphereMat2{ new Reflective() };
    std::dynamic_pointer_cast<Phong>(sphereMat2)->SetKa(0.0);
    std::dynamic_pointer_cast<Phong>(sphereMat2)->SetKd(0.3);
    std::dynamic_pointer_cast<Phong>(sphereMat2)->SetKs(0.0);
    std::dynamic_pointer_cast<Reflective>(sphereMat2)->SetCr(RGBColor(0.7, 0.7, 1.0));
    std::dynamic_pointer_cast<Reflective>(sphereMat2)->SetKr(0.8);
    sphere2->SetMaterial(sphereMat2);

    std::shared_ptr<GeometricObject> comp{ new RayTracer::Grid() };

    std::dynamic_pointer_cast<RayTracer::Grid>(comp)->AddObject(lightplane1);
    std::dynamic_pointer_cast<RayTracer::Grid>(comp)->AddObject(lightplane2);
    std::dynamic_pointer_cast<RayTracer::Grid>(comp)->AddObject(lightplane3);
    std::dynamic_pointer_cast<RayTracer::Grid>(comp)->AddObject(plane);
    std::dynamic_pointer_cast<RayTracer::Grid>(comp)->AddObject(box1);
    std::dynamic_pointer_cast<RayTracer::Grid>(comp)->AddObject(box2);
    std::dynamic_pointer_cast<RayTracer::Grid>(comp)->AddObject(box3);
    std::dynamic_pointer_cast<RayTracer::Grid>(comp)->AddObject(box4);
    std::dynamic_pointer_cast<RayTracer::Grid>(comp)->AddObject(sphere1);
    std::dynamic_pointer_cast<RayTracer::Grid>(comp)->AddObject(sphere2);

    std::dynamic_pointer_cast<RayTracer::Grid>(comp)->Setup();

    world->AddObject(comp);
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
        menu.addCommandItem(&commandManager, CommandIDs::settingWorld);
        menu.addCommandItem(&commandManager, CommandIDs::settingViewplane);
        menu.addCommandItem(&commandManager, CommandIDs::settingCamera);
        menu.addCommandItem(&commandManager, CommandIDs::settingLight);
        menu.addCommandItem(&commandManager, CommandIDs::settingGeometry);
        menu.addCommandItem(&commandManager, CommandIDs::settingMaterial);
        menu.addCommandItem(&commandManager, CommandIDs::settingMesh);
    }
    else if (menuIndex == 1)
    {
        menu.addCommandItem(&commandManager, CommandIDs::startRender);
        menu.addCommandItem(&commandManager, CommandIDs::stopRender);
		menu.addCommandItem(&commandManager, CommandIDs::saveRender);
        menu.addCommandItem(&commandManager, CommandIDs::showHDR);
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
    Array<CommandID> commands{ CommandIDs::startRender,
        CommandIDs::stopRender,
        CommandIDs::saveRender,
        CommandIDs::showHDR,
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
        case CommandIDs::stopRender:
            result.setInfo ("Stop", "Stop the rendering process", "Render", 0);
            break;
		case CommandIDs::saveRender:
			result.setInfo ("Save", "Save rendered image", "Render", 0);
#if JUCE_MAC
			result.addDefaultKeypress('s', ModifierKeys::commandModifier);
#else
			result.addDefaultKeypress('s', ModifierKeys::ctrlModifier);
#endif
			break;
        case CommandIDs::showHDR:
            result.setInfo("Remap Tone", "Convert HDR image to normal range", "Render", 0);
            result.setTicked(isShowHDR);
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
			progress = -1.0;
			rendering = true;
			progressBar->setSize(0, 25);
#if JUCE_MAC
			setSize(std::max(100, viewPlane->Width), std::max(100, viewPlane->Height) + progressBar->getHeight());
#else
			setSize(std::max(100, viewPlane->Width), std::max(100, viewPlane->Height) + progressBar->getHeight() + LookAndFeel::getDefaultLookAndFeel().getDefaultMenuBarHeight());
#endif
			progressBar->setVisible(true);
			repaint();
			renderer.Render(progress, camera, tracer, viewPlane);
            break;
        case CommandIDs::stopRender:
			renderer.Cancel();
			break;
		case CommandIDs::saveRender:
		{
			FileChooser chooser("Save", File(), "*.png");
			if (chooser.browseForFileToSave(true))
			{
				File selectedFile = chooser.getResult();
				FileOutputStream stream(selectedFile);
				stream.setPosition(0);
				stream.truncate();
				PNGImageFormat pngWriter;
				pngWriter.writeImageToStream(*viewPlane->RenderedImage, stream);
			}
			break;
		}
        case CommandIDs::showHDR:
            isShowHDR = !isShowHDR;
            if (isShowHDR)
            {
                viewPlane->ShowHDR();
            }
            else
            {
                viewPlane->ShowClamped();
            }
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
    progressBar->setBounds(b.removeFromBottom(progressBar->getHeight()));
    image->setBounds(b);
}

//==============================================================================
void MainComponent::renderSucceeded(FP_TYPE timeElapsed)
{
	AlertWindow::showMessageBox(AlertWindow::AlertIconType::InfoIcon, "Rendering Finished", "The rendering took " + std::to_string(timeElapsed) + "s.");
	DBG(timeElapsed);
	progress = -1.0;
	rendering = false;
	renderFinished = true;
}

void MainComponent::timerCallback()
{
	const MessageManagerLock mmLock;
	if (++timerSkip % 3 == 0)
	{
		timerSkip = 0;
		image->setImage(*viewPlane->RenderedImage);
		image->repaint();
	}
	if (renderFinished)
	{
		renderFinished = false;
		progressBar->setSize(0, 0);
		progressBar->setVisible(false);
#if JUCE_MAC
		setSize(std::max(100, viewPlane->Width), std::max(100, viewPlane->Height));
#else
		setSize(std::max(100, viewPlane->Width), std::max(100, viewPlane->Height) + LookAndFeel::getDefaultLookAndFeel().getDefaultMenuBarHeight());
#endif
		repaint();
	}
}
