/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    auto vpWidth = 72 * 5, vpHeight = 72 * 5;
    world.reset(new World());
    tracer.reset(new RayCast(world));
    viewPlane.reset(new ViewPlane(vpWidth, vpHeight, 1.0 / vpHeight, 16));
    sampler.reset(new PreviewSampler());
    
    auto r = 3.7;
    auto theta = 30.0 * PI_OVER_180;
    auto phi = 45 * PI_OVER_180;
    auto roll = 0.0 * PI_OVER_180;
    auto lookat = Vec3D(0.0, 0.0, 0.0);
    auto eyepoint = Vec3D(r * sin(theta) * sin(phi), r * cos(phi), r * cos(theta) * sin(phi)) + lookat;
	camera.reset(new PinholeCamera(eyepoint, lookat, Vec3D(sin(roll), cos(roll), 0.0), 1.0, viewPlane, sampler));
    
	std::shared_ptr<Light> parallelLight{ new ParallelLight(3.0, RGBColor(1.0, 1.0, 1.0), Vec3D(-1.0, -1.0, 0.5)) };
    //world->AddLight(parallelLight);
    
	std::shared_ptr<Light> l2{ new PointLight(10.0, Vec3D(1, 0, 0), Point3D(-2, 5, 0)) };
	world->AddLight(l2);
	std::shared_ptr<Light> l3{ new PointLight(10.0, Vec3D(0, 1, 0), Point3D(2, 5, 0)) };
	world->AddLight(l3);
	std::shared_ptr<Light> l4{ new PointLight(10.0, Vec3D(0, 0, 1), Point3D(0, 5, 2)) };
	world->AddLight(l4);

	std::shared_ptr<Light> ambient{ new Ambient(0.8, RGBColor(1.0, 1.0, 1.0)) };
	world->SetAmbient(ambient);

	std::shared_ptr<GeometricObject> plane{ new Plane(Point3D(-1, 0, 1), Vec3D(2, 0, 0), Vec3D(0, 0, -2)) };
	std::shared_ptr<Material> matte{ new Matte() };
	std::dynamic_pointer_cast<Matte>(matte)->SetKa(0.25);
	std::dynamic_pointer_cast<Matte>(matte)->SetKd(0.75);
	std::dynamic_pointer_cast<Matte>(matte)->SetCd(WHITE);
	plane->SetMaterial(matte);
	std::shared_ptr<GeometricObject> plins{ new Instance(plane) };

	//std::shared_ptr<GeometricObject> comp{ new Compound() };
	std::shared_ptr<GeometricObject> comp{ new RayTracer::Grid() };

	std::dynamic_pointer_cast<RayTracer::Grid>(comp)->AddObject(plins);

	File file("D:\\dragon.obj");
	std::shared_ptr<GeometricObject> mesh;
	if (file.existsAsFile())
	{
		OBJParser parser;
		StringArray strarr;
		file.readLines(strarr);
		mesh = std::make_shared<Mesh>(parser.parse(strarr));
	}

	int numSphere = 10;
	float volume = 0.1 / numSphere;
	float radius = pow(0.75 * volume / PI, 1.0 / 3);
	//std::dynamic_pointer_cast<Instance>(plins)->Translate(0, -1, 0);
	Random random;

	/*for (int i = 0; i < numSphere; i++)
	{
		std::shared_ptr<Material> phong{ new Phong() };
		std::dynamic_pointer_cast<Phong>(phong)->SetKa(0.25);
		std::dynamic_pointer_cast<Phong>(phong)->SetKd(0.75);
		std::dynamic_pointer_cast<Phong>(phong)->SetKs(0.4);
		std::dynamic_pointer_cast<Phong>(phong)->SetE(80.0);
		std::dynamic_pointer_cast<Phong>(phong)->SetCd(RGBColor(random.nextDouble(), random.nextDouble(), random.nextDouble()));
		std::dynamic_pointer_cast<Phong>(phong)->SetCs(RGBColor(1.0, 1.0, 1.0));

		std::shared_ptr<GeometricObject> sphere{ new Sphere(Point3D(1 - 2.0 * random.nextDouble(), 1 - 2.0 * random.nextDouble(), 1 - 2.0 * random.nextDouble()), radius) };
		sphere->SetMaterial(phong);
		std::dynamic_pointer_cast<Compound>(comp)->AddObject(sphere);
	}*/

	int numberAx = 1;
	std::dynamic_pointer_cast<Instance>(plins)->Translate(0, mesh->GetBoundingBox().GetMinPoint().y, 0);
	for (int i = 0; i < numberAx; i++)
	{
		for (int k = 0; k < numberAx; k++)
		{
			std::shared_ptr<Material> phong{ new Phong() };
			std::dynamic_pointer_cast<Phong>(phong)->SetKa(0.25);
			std::dynamic_pointer_cast<Phong>(phong)->SetKd(0.75);
			std::dynamic_pointer_cast<Phong>(phong)->SetKs(0.4);
			std::dynamic_pointer_cast<Phong>(phong)->SetE(80.0);
			std::dynamic_pointer_cast<Phong>(phong)->SetCd(RGBColor(random.nextDouble(), random.nextDouble(), random.nextDouble()));
			std::dynamic_pointer_cast<Phong>(phong)->SetCs(RGBColor(1.0, 1.0, 1.0));
			mesh->SetMaterial(phong);

			std::shared_ptr<GeometricObject> ins{ new Instance(mesh) };
			std::dynamic_pointer_cast<Instance>(ins)->Scale(1, 1, 1);
			std::dynamic_pointer_cast<Instance>(ins)->Translate(0, 0, 0);
			std::dynamic_pointer_cast<RayTracer::Grid>(comp)->AddObject(ins);
		}
	}


	std::dynamic_pointer_cast<RayTracer::Grid>(comp)->Setup();
	world->AddObject(comp);

	renderer.Render(camera, tracer, viewPlane);
    
    progress = 0.5;
    
    menuBar.reset(new MenuBarComponent(this));
    image.reset(new ImageComponent());
    progressBar.reset(new ProgressBar(progress));
    
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
    setSize(vpWidth, vpHeight + progressBar->getHeight());
#else
    setSize(vpWidth, vpHeight + progressBar->getHeight() + LookAndFeel::getDefaultLookAndFeel().getDefaultMenuBarHeight());
#endif
	//World world;
    //FileOutputStream stream(File("/Volumes/Document/RayTracer/test.png"));
    //PNGImageFormat pngWriter;
    //pngWriter.writeImageToStream(*viewPlane->RenderedImage, stream);
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
void MainComponent::renderSucceeded()
{
	DBG("Finished");
	const MessageManagerLock mmLock;
	image->setImage(*viewPlane->RenderedImage);
	progress = 1.0;
	repaint();
}

void MainComponent::timerCallback()
{
	const MessageManagerLock mmLock;
	progress = renderer.GetProgress();
	image->setImage(*viewPlane->RenderedImage);
	repaint();
}
