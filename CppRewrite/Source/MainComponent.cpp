/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    auto vpWidth = 128 * 5, vpHeight = 72 * 5;
    world.reset(new World());
    tracer.reset(new RayCast(world));
    viewPlane.reset(new ViewPlane(vpWidth, vpHeight, 1.0 / vpHeight, 16));
    sampler.reset(new PreviewSampler());
    
    auto r = 4.0;
    auto theta = 45.0 / 180.0 * 3.1416;
    auto phi = 45.0 / 180.0 * 3.1416;
    auto roll = 0.0 / 180.0 * 3.1416;
    auto lookat = Vec3D(0.0, -0.6, -1.5);
    auto eyepoint = Vec3D(r * sin(theta) * sin(phi), r * cos(phi), r * cos(theta) * sin(phi)) + lookat;
    //camera.reset(new OrthographicCamera(eyepoint, lookat, Vec3D(sin(roll), cos(roll), 0.0), viewPlane, sampler));
	camera.reset(new PinholeCamera(eyepoint, lookat, Vec3D(sin(roll), cos(roll), 0.0), 1.0, viewPlane, sampler));
    
	std::shared_ptr<Light> l1{ new ParallelLight(1.0, RGBColor(1.0, 1.0, 1.0), Vec3D(-1.0, -1.0, 0.5)) };
    world->AddLight(l1);
    
    std::shared_ptr<Light> l2{ new PointLight(3.0, WHITE, Point3D(0.0, 0.0, 0.0)) };
    world->AddLight(l2);

	std::shared_ptr<Light> amb{ new Ambient(0.05, RGBColor(1.0, 1.0, 1.0)) };
	world->SetAmbient(amb);
    
    
	std::shared_ptr<GeometricObject> sp1{ new Sphere(Vec3D(0.0, 0.0, -2.0), 0.5) };
	std::shared_ptr<Material> mat1{ new Matte() };
	std::dynamic_pointer_cast<Matte>(mat1)->SetKa(0.25);
	std::dynamic_pointer_cast<Matte>(mat1)->SetKd(1.0);
	std::dynamic_pointer_cast<Matte>(mat1)->SetCd(RGBColor(0.0, 1.0, 0.0));
    sp1->SetMaterial(mat1);
    world->AddObject(sp1);

	std::shared_ptr<GeometricObject> sp2{ new Sphere(Vec3D(-1.0, 0.0, -1.0), 0.5) };
	std::shared_ptr<Material> mat2{ new Matte() };
	std::dynamic_pointer_cast<Matte>(mat2)->SetKa(0.25);
	std::dynamic_pointer_cast<Matte>(mat2)->SetKd(1.0);
	std::dynamic_pointer_cast<Matte>(mat2)->SetCd(RGBColor(1.0, 0.0, 0.0));
	sp2->SetMaterial(mat2);
	world->AddObject(sp2);

	std::shared_ptr<GeometricObject> sp3{ new Sphere(Vec3D(1.0, 0.0, -1.0), 0.5) };
	std::shared_ptr<Material> mat3{ new Matte() };
	std::dynamic_pointer_cast<Matte>(mat3)->SetKa(0.25);
	std::dynamic_pointer_cast<Matte>(mat3)->SetKd(1.0);
	std::dynamic_pointer_cast<Matte>(mat3)->SetCd(RGBColor(0.0, 0.0, 1.0));
	sp3->SetMaterial(mat3);
	world->AddObject(sp3);

	std::shared_ptr<GeometricObject> pl1{ new Plane(Point3D(-2.0, -0.6, 0.0), Vec3D(4.0, 0.0, 0.0), Vec3D(0.0, 0.0, -3.0)) };
	std::shared_ptr<Material> mat4{ new Matte() };
	std::dynamic_pointer_cast<Matte>(mat4)->SetKa(0.25);
	std::dynamic_pointer_cast<Matte>(mat4)->SetKd(0.6);
	std::dynamic_pointer_cast<Matte>(mat4)->SetCd(RGBColor(1.0, 1.0, 1.0));
	pl1->SetMaterial(mat4);
	world->AddObject(pl1);

	std::shared_ptr<GeometricObject> pl2{ new Plane(Point3D(-0.4, -0.4, -0.5), Vec3D(0.0, 0.0, -1.0), Vec3D(0.0, 0.7, 0.0)) };
	std::shared_ptr<Material> mat5{ new Matte() };
	std::dynamic_pointer_cast<Matte>(mat5)->SetKa(0.25);
	std::dynamic_pointer_cast<Matte>(mat5)->SetKd(0.8);
	std::dynamic_pointer_cast<Matte>(mat5)->SetCd(RGBColor(0.0, 1.0, 1.0));
	pl2->SetMaterial(mat5);
	world->AddObject(pl2);

	std::shared_ptr<GeometricObject> tri{ new Triangle(Point3D(0.0, -0.5, -1.0), Point3D(1.5, -0.3, -2.5), Point3D(0.5, 0.8, -1.5)) };
	std::shared_ptr<Material> mat6{ new Matte() };
	std::dynamic_pointer_cast<Matte>(mat6)->SetKa(0.25);
	std::dynamic_pointer_cast<Matte>(mat6)->SetKd(0.8);
	std::dynamic_pointer_cast<Matte>(mat6)->SetCd(RGBColor(0.0, 1.0, 1.0));
	tri->SetMaterial(mat6);
	world->AddObject(tri);
    
    for (int r = 0; r < vpHeight; r++)
    {
        for (int c = 0; c < vpWidth; c++)
        {
            auto rays = camera->CreateRay(c, r);
            for (auto& ray : rays)
            {
                viewPlane->SetPixel(c, r, tracer->Trace(ray));
            }
        }
        DBG(r);
    }
    
    progress = 0.5;
    
    menuBar.reset(new MenuBarComponent(this));
    image.reset(new ImageComponent());
    progressBar.reset(new ProgressBar(progress));
    
    progressBar->setSize(0, 25);
    image->setImage(*viewPlane->RenderedImage);
    
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
