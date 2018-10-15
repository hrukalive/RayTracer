/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    auto vpWidth = 512, vpHeight = 512;
    world.reset(new World());
    tracer.reset(new RayCast(world));
    viewPlane.reset(new ViewPlane(vpWidth, vpHeight, (FP_TYPE)(1.0 / vpHeight), 32));
    sampler.reset(new PreviewSampler());
    
	auto r = 3.7; //2.8
    auto theta = 30.0 * PI_OVER_180;
    auto phi = 45 * PI_OVER_180;
    auto roll = 0.0 * PI_OVER_180;
    auto lookat = Vec3D(0.0, 0.0, 0.0);
    auto eyepoint = Vec3D(r * sin(theta) * sin(phi), r * cos(phi), r * cos(theta) * sin(phi)) + lookat;
	camera.reset(new PinholeCamera(eyepoint, lookat, Vec3D(sin(roll), cos(roll), 0.0), 1.0, viewPlane, sampler));
    
	std::shared_ptr<Light> parallelLight{ new ParallelLight(3.0, RGBColor(1.0, 1.0, 1.0), Vec3D(-1.0, -1.0, 0.5)) };
    world->AddLight(parallelLight);
    
	std::shared_ptr<Light> l2{ new PointLight(5, Vec3D(1, 0, 0), Point3D(-2, 1, 0)) };
	world->AddLight(l2);
	std::shared_ptr<Light> l3{ new PointLight(5, Vec3D(0, 1, 0), Point3D(2, 1, 0)) };
	world->AddLight(l3);
	std::shared_ptr<Light> l4{ new PointLight(5, Vec3D(0, 0, 1), Point3D(0, 1, 2)) };
	world->AddLight(l4);

	std::shared_ptr<Light> ambient{ new Ambient(0.8, RGBColor(1.0, 1.0, 1.0)) };
	world->SetAmbient(ambient);

	std::shared_ptr<GeometricObject> plane{ new Plane(Point3D(-3, 0, 3), Vec3D(6, 0, 0), Vec3D(0, 0, -6)) };
	std::shared_ptr<Material> matte{ new Matte() };
	std::dynamic_pointer_cast<Matte>(matte)->SetKa(0.0);
	std::dynamic_pointer_cast<Matte>(matte)->SetKd(0.5);
	std::dynamic_pointer_cast<Matte>(matte)->SetCd(WHITE);
	plane->SetMaterial(matte);
	std::shared_ptr<GeometricObject> plins{ new Instance(plane) };

	//std::shared_ptr<GeometricObject> comp{ new Compound() };
	std::shared_ptr<GeometricObject> comp{ new RayTracer::Grid() };

	std::dynamic_pointer_cast<Compound>(comp)->AddObject(plins);

    File file("/Volumes/Document/dragon.obj");
    std::shared_ptr<GeometricObject> mesh;
    if (file.existsAsFile())
    {
        OBJParser parser;
        StringArray strarr;
        file.readLines(strarr);
        mesh = std::make_shared<Mesh>(parser.parse(strarr));
    }

	int numSphere = 100;
    FP_TYPE volume = (FP_TYPE)(0.1 / numSphere);
    FP_TYPE radius = (FP_TYPE)pow(0.75 * volume / PI, 1.0 / 3);
	//std::dynamic_pointer_cast<Instance>(plins)->Translate(0, -1, 0);
	Random random;

	//for (int i = 0; i < numSphere; i++)
	//{
	//	std::shared_ptr<Material> phong{ new Phong() };
	//	std::dynamic_pointer_cast<Phong>(phong)->SetKa(0.25);
	//	std::dynamic_pointer_cast<Phong>(phong)->SetKd(0.75);
	//	std::dynamic_pointer_cast<Phong>(phong)->SetKs(0.4);
	//	std::dynamic_pointer_cast<Phong>(phong)->SetE(80.0);
	//	std::dynamic_pointer_cast<Phong>(phong)->SetCd(RGBColor(random.nextDouble(), random.nextDouble(), random.nextDouble()));
	//	std::dynamic_pointer_cast<Phong>(phong)->SetCs(RGBColor(1.0, 1.0, 1.0));

	//	std::shared_ptr<GeometricObject> sphere{ new Sphere(Point3D(1 - 2.0 * random.nextDouble(), 1 - 2.0 * random.nextDouble(), 1 - 2.0 * random.nextDouble()), radius) };
	//	sphere->SetMaterial(phong);
	//	std::dynamic_pointer_cast<Compound>(comp)->AddObject(sphere);
	//}

    std::dynamic_pointer_cast<Instance>(plins)->Translate(0, mesh->GetBoundingBox().GetMinPoint().y, 0);
    std::shared_ptr<Material> phong{ new Phong() };
    std::dynamic_pointer_cast<Phong>(phong)->SetKa(0.25);
    std::dynamic_pointer_cast<Phong>(phong)->SetKd(0.3);
    std::dynamic_pointer_cast<Phong>(phong)->SetKs(0.4);
    std::dynamic_pointer_cast<Phong>(phong)->SetE(80.0);
    std::dynamic_pointer_cast<Phong>(phong)->SetCd(RGBColor(1.0, 1.0, 1.0));
    std::dynamic_pointer_cast<Phong>(phong)->SetCs(RGBColor(1.0, 1.0, 1.0));
    mesh->SetMaterial(phong);

    std::shared_ptr<GeometricObject> ins{ new Instance(mesh) };
    std::dynamic_pointer_cast<Instance>(ins)->Scale(1, 1, 1);
    std::dynamic_pointer_cast<Instance>(ins)->Translate(0, 0, 0);
	std::dynamic_pointer_cast<Compound>(comp)->AddObject(ins);


	std::dynamic_pointer_cast<RayTracer::Grid>(comp)->Setup();
	world->AddObject(comp);
    
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
