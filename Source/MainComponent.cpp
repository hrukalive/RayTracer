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
    tracer.reset(new PhotonMapTrace(10));
    viewPlane.reset(new ViewPlane(vpWidth, vpHeight, (FP_TYPE)(1.0 / vpHeight), 8, 4, false));
    sampler.reset(new MultiJittered(viewPlane->NumPixelSamples));
    photonMap = createPhotonMap(TOTAL_PHOTON);
    
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
    setSize(std::max(100, viewPlane->isStereo ? 2 * vpWidth : vpWidth), std::max(100, vpHeight));
#else
    setSize(std::max(100, viewPlane->isStereo ? 2 * vpWidth : vpWidth), std::max(100, vpHeight) + LookAndFeel::getDefaultLookAndFeel().getDefaultMenuBarHeight());
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
    if (balancedPhotonMap != nullptr)
        destroyPhotonMap(balancedPhotonMap);
    else
    {
        free(photonMap->photons);
        free(photonMap);
    }

#if JUCE_MAC
    MenuBarModel::setMacMainMenu (nullptr);
#endif
}

/*
void MainComponent::setupWorld()
{
    auto r = 3.7; //2.8
    auto theta = 0.0 * PI_OVER_180;
    auto phi = 90 * PI_OVER_180;
    auto roll = 0.0 * PI_OVER_180;
    auto lookat = Vec3D(0.0, 0.0, 0.0);
    auto eyepoint = Vec3D(r * sin(theta) * sin(phi), r * cos(phi), r * cos(theta) * sin(phi)) + lookat;

    //camera.reset(new ThinLensCamera(eyepoint, lookat, Vec3D(sin(roll), cos(roll), 0.0), 1.0, 4, 0.3));
    std::shared_ptr<Camera> cam1{ new PinholeCamera(eyepoint, lookat, Vec3D(sin(roll), cos(roll), 0.0), 1.0) };
    std::shared_ptr<Camera> cam2{ new PinholeCamera(eyepoint, lookat, Vec3D(sin(roll), cos(roll), 0.0), 1.0) };
    camera.reset(new StereoCamera(eyepoint, lookat, Vec3D(sin(roll), cos(roll), 0.0), cam1, cam2, 5, true));

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

    std::shared_ptr<GeometricObject> comp{ new RayTracer::Grid() };

    int numSphere = 100;
    FP_TYPE volume = (FP_TYPE)(0.1 / numSphere);
    FP_TYPE radius = (FP_TYPE)pow(0.75 * volume / PI, 1.0 / 3);
    Random random;

    for (int i = 0; i < numSphere; i++)
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
        std::dynamic_pointer_cast<RayTracer::Grid>(comp)->AddObject(sphere);
    }

    std::dynamic_pointer_cast<RayTracer::Grid>(comp)->Setup();
    world->AddObject(comp);
}
*/

/*
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
    std::dynamic_pointer_cast<Emissive>(lightMat2)->SetLs(25);
    std::dynamic_pointer_cast<Emissive>(lightMat2)->SetCe(RGBColor(1.0, 1.0, 0.5));
    lightplane2->SetMaterial(lightMat2);
    std::shared_ptr<Light> arealight2{ new AreaLight(lightplane2, lightMat2) };
    world->AddLight(arealight2);
    
    std::shared_ptr<GeometricObject> lightplane3{ new RayTracer::Rectangle(Point3D(-1.4, 0.9, 0.0), Vec3D(0, 0.5, 0), Vec3D(0, 0, 0.6)) };
    std::shared_ptr<Material> lightMat3{ new Emissive() };
    std::dynamic_pointer_cast<Emissive>(lightMat3)->SetLs(25);
    std::dynamic_pointer_cast<Emissive>(lightMat3)->SetCe(RGBColor(0.5, 1.0, 1.0));
    lightplane3->SetMaterial(lightMat3);
    std::shared_ptr<Light> arealight3{ new AreaLight(lightplane3, lightMat3) };
    world->AddLight(arealight3);

    std::shared_ptr<GeometricObject> plane{ new RayTracer::Rectangle(Point3D(-3, 0, 3), Vec3D(6, 0, 0), Vec3D(0, 0, -6)) };
    std::shared_ptr<Material> planeMat{ new GlossyReflector() };
    std::dynamic_pointer_cast<GlossyReflector>(planeMat)->SetKa(0.1);
    std::dynamic_pointer_cast<GlossyReflector>(planeMat)->SetKd(0.4);
    std::dynamic_pointer_cast<GlossyReflector>(planeMat)->SetKs(0.4);
    std::dynamic_pointer_cast<GlossyReflector>(planeMat)->SetCd(RGBColor(1.0, 1.0, 1.0));
    std::dynamic_pointer_cast<GlossyReflector>(planeMat)->SetCs(RGBColor(1.0, 1.0, 1.0));
    std::dynamic_pointer_cast<GlossyReflector>(planeMat)->SetE(500.0);
    plane->SetMaterial(planeMat);

    std::shared_ptr<GeometricObject> box1{ new Box(Point3D(-1, 0, 0), Point3D(-0.7, 1, 0.3)) };
    std::shared_ptr<Material> boxMat1{ new Reflective() };
    std::dynamic_pointer_cast<Reflective>(boxMat1)->SetKa(0.1);
    std::dynamic_pointer_cast<Reflective>(boxMat1)->SetKd(0.1);
    std::dynamic_pointer_cast<Reflective>(boxMat1)->SetCd(RGBColor(0.0, 1.0, 0.8));
    std::dynamic_pointer_cast<Reflective>(boxMat1)->SetCr(RGBColor(0.8, 1.0, 0.8));
    std::dynamic_pointer_cast<Reflective>(boxMat1)->SetKr(0.8);
    box1->SetMaterial(boxMat1);
    
    std::shared_ptr<GeometricObject> box2{ new Box(Point3D(-0.43, 0, 0.1), Point3D(-0.13, 1, 0.4)) };
    std::shared_ptr<Material> boxMat2{ new Transparent() };
    std::dynamic_pointer_cast<Transparent>(boxMat2)->SetKa(0.1);
    std::dynamic_pointer_cast<Transparent>(boxMat2)->SetKd(0.1);
    std::dynamic_pointer_cast<Transparent>(boxMat2)->SetKs(0.0);
    std::dynamic_pointer_cast<Transparent>(boxMat2)->SetCd(RGBColor(1.0, 0.0, 0.8));
    std::dynamic_pointer_cast<Transparent>(boxMat2)->SetCr(RGBColor(1.0, 0.8, 0.8));
    std::dynamic_pointer_cast<Transparent>(boxMat2)->SetCt(RGBColor(1.0, 0.8, 0.8));
    std::dynamic_pointer_cast<Transparent>(boxMat2)->SetKr(0.5);
    std::dynamic_pointer_cast<Transparent>(boxMat2)->SetKt(1.0);
    std::dynamic_pointer_cast<Transparent>(boxMat2)->SetIOR(1.15);
    box2->SetMaterial(boxMat2);
    
    std::shared_ptr<GeometricObject> box3{ new Box(Point3D(0.13, 0, 0), Point3D(0.43, 1, 0.3)) };
    std::shared_ptr<Material> boxMat3{ new Reflective() };
    std::dynamic_pointer_cast<Reflective>(boxMat3)->SetKa(0.1);
    std::dynamic_pointer_cast<Reflective>(boxMat3)->SetKd(0.1);
    std::dynamic_pointer_cast<Reflective>(boxMat3)->SetCd(RGBColor(0.8, 1.0, 0.0));
    std::dynamic_pointer_cast<Reflective>(boxMat3)->SetCr(RGBColor(0.8, 1.0, 0.8));
    std::dynamic_pointer_cast<Reflective>(boxMat3)->SetKr(0.8);
    box3->SetMaterial(boxMat3);

    std::shared_ptr<GeometricObject> box4{ new Box(Point3D(0.7, 0, 0.1), Point3D(1, 1, 0.4)) };
    std::shared_ptr<Material> boxMat4{ new Transparent() };
    std::dynamic_pointer_cast<Transparent>(boxMat4)->SetKa(0.1);
    std::dynamic_pointer_cast<Transparent>(boxMat4)->SetKd(0.3);
    std::dynamic_pointer_cast<Transparent>(boxMat4)->SetKs(0.0);
    std::dynamic_pointer_cast<Transparent>(boxMat4)->SetCr(RGBColor(0.7, 1.0, 0.7));
    std::dynamic_pointer_cast<Transparent>(boxMat4)->SetCt(RGBColor(0.8, 1.0, 0.8));
    std::dynamic_pointer_cast<Transparent>(boxMat4)->SetKr(0.5);
    std::dynamic_pointer_cast<Transparent>(boxMat4)->SetKt(1.0);
    std::dynamic_pointer_cast<Transparent>(boxMat4)->SetIOR(1.1);
    box4->SetMaterial(boxMat4);

    std::shared_ptr<GeometricObject> box5{ new Box(Point3D(-1, 0, -2.5), Point3D(-0.7, 1, -2.2)) };
    std::shared_ptr<GeometricObject> box6{ new Box(Point3D(-0.43, 0, -2.5), Point3D(-0.13, 1, -2.2)) };
    std::shared_ptr<GeometricObject> box7{ new Box(Point3D(0.13, 0, -2.5), Point3D(0.43, 1, -2.2)) };
    std::shared_ptr<GeometricObject> box8{ new Box(Point3D(0.7, 0, -2.5), Point3D(1, 1, -2.2)) };
    std::shared_ptr<Material> boxMatNew{ new Phong() };
    std::dynamic_pointer_cast<Phong>(boxMatNew)->SetKa(0.3);
    std::dynamic_pointer_cast<Phong>(boxMatNew)->SetKd(0.3);
    std::dynamic_pointer_cast<Phong>(boxMatNew)->SetKs(0.3);
    std::dynamic_pointer_cast<Phong>(boxMatNew)->SetE(80.0);
    std::dynamic_pointer_cast<Phong>(boxMatNew)->SetCd(RGBColor(0.8, 1.0, 0.0));
    std::dynamic_pointer_cast<Phong>(boxMatNew)->SetCs(RGBColor(1.0, 1.0, 1.0));
    box5->SetMaterial(boxMatNew);
    box6->SetMaterial(boxMatNew);
    box7->SetMaterial(boxMatNew);
    box8->SetMaterial(boxMatNew);
    
    std::shared_ptr<GeometricObject> sphere1{ new Sphere(Point3D(-0.3, 0.4, -0.55), 0.4) };
    std::shared_ptr<Material> sphereMat1{ new Reflective() };
    std::dynamic_pointer_cast<Reflective>(sphereMat1)->SetKa(0.0);
    std::dynamic_pointer_cast<Reflective>(sphereMat1)->SetKd(0.3);
    std::dynamic_pointer_cast<Reflective>(sphereMat1)->SetCr(RGBColor(1.0, 0.7, 0.7));
    std::dynamic_pointer_cast<Reflective>(sphereMat1)->SetKr(0.8);
    sphere1->SetMaterial(sphereMat1);
    
    std::shared_ptr<GeometricObject> sphere2{ new Sphere(Point3D(0.9, 0.4, -0.55), 0.4) };
    std::shared_ptr<Material> sphereMat2{ new Transparent() };
    std::dynamic_pointer_cast<Transparent>(sphereMat2)->SetKa(0.0);
    std::dynamic_pointer_cast<Transparent>(sphereMat2)->SetKd(0.3);
    std::dynamic_pointer_cast<Transparent>(sphereMat2)->SetKs(0.0);
    std::dynamic_pointer_cast<Transparent>(sphereMat2)->SetCr(RGBColor(0.7, 0.7, 1.0));
    std::dynamic_pointer_cast<Transparent>(sphereMat2)->SetCt(RGBColor(0.7, 0.7, 1.0));
    std::dynamic_pointer_cast<Transparent>(sphereMat2)->SetKr(0.5);
    std::dynamic_pointer_cast<Transparent>(sphereMat2)->SetKt(0.9);
    std::dynamic_pointer_cast<Transparent>(sphereMat2)->SetIOR(1.25);
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
    std::dynamic_pointer_cast<RayTracer::Grid>(comp)->AddObject(box5);
    std::dynamic_pointer_cast<RayTracer::Grid>(comp)->AddObject(box6);
    std::dynamic_pointer_cast<RayTracer::Grid>(comp)->AddObject(box7);
    std::dynamic_pointer_cast<RayTracer::Grid>(comp)->AddObject(box8);
    std::dynamic_pointer_cast<RayTracer::Grid>(comp)->AddObject(sphere1);
    std::dynamic_pointer_cast<RayTracer::Grid>(comp)->AddObject(sphere2);

    std::dynamic_pointer_cast<RayTracer::Grid>(comp)->Setup();

    world->AddObject(comp);
}
*/


void MainComponent::setupWorld()
{
    auto r = 160;
    auto theta = 0 * PI_OVER_180;
    auto phi = 90 * PI_OVER_180;
    auto roll = 0.0 * PI_OVER_180;
    auto lookat = Vec3D(0.0, 0.0, 0.0);
    auto eyepoint = Vec3D(r * sin(theta) * sin(phi), r * cos(phi), r * cos(theta) * sin(phi)) + lookat;
    camera.reset(new PinholeCamera(eyepoint, lookat, Vec3D(sin(roll), cos(roll), 0.0), 1.0));
    //std::shared_ptr<Camera> cam1{ new PinholeCamera(eyepoint, lookat, Vec3D(sin(roll), cos(roll), 0.0), 1.0) };
    //std::shared_ptr<Camera> cam2{ new PinholeCamera(eyepoint, lookat, Vec3D(sin(roll), cos(roll), 0.0), 1.0) };
    //camera.reset(new StereoCamera(eyepoint, lookat, Vec3D(sin(roll), cos(roll), 0.0), cam1, cam2, 5, true));

    std::shared_ptr<Light> ambient{ new Ambient(2.0, RGBColor(1.0, 1.0, 1.0)) };
    world->SetAmbient(ambient);

    std::shared_ptr<GeometricObject> lightplane1{ new RayTracer::Rectangle(Point3D(-10, 49, 5), Vec3D(0, 0, -10), Vec3D(20, 0, 0)) };
    std::shared_ptr<Material> lightMat1{ new Emissive() };
    std::dynamic_pointer_cast<Emissive>(lightMat1)->SetLs(100000);
    std::dynamic_pointer_cast<Emissive>(lightMat1)->SetCe(RGBColor(0.98039, 0.80784, 0.59608));
    lightplane1->SetMaterial(lightMat1);
    std::shared_ptr<Light> arealight1{ new AreaLight(lightplane1, lightMat1) };
    world->AddLight(arealight1);

    std::shared_ptr<GeometricObject> planeBottom{ new RayTracer::Rectangle(Point3D(-50, -50, 50), Vec3D(100, 0, 0), Vec3D(0, 0, -100)) };
    std::shared_ptr<Material> planeMat1{ new Matte() };
    std::dynamic_pointer_cast<Matte>(planeMat1)->SetKa(0.2);
    std::dynamic_pointer_cast<Matte>(planeMat1)->SetKd(0.9);
    std::dynamic_pointer_cast<Matte>(planeMat1)->SetCd(RGBColor(1, 1, 1));
    planeBottom->SetMaterial(planeMat1);

    std::shared_ptr<GeometricObject> planeTop{ new RayTracer::Rectangle(Point3D(-50, 50, 50), Vec3D(0, 0, -100), Vec3D(100, 0, 0)) };
    std::shared_ptr<Material> planeMat2{ new Matte() };
    std::dynamic_pointer_cast<Matte>(planeMat2)->SetKa(0.2);
    std::dynamic_pointer_cast<Matte>(planeMat2)->SetKd(0.9);
    std::dynamic_pointer_cast<Matte>(planeMat2)->SetCd(RGBColor(1, 1, 1));
    planeTop->SetMaterial(planeMat2);

    std::shared_ptr<GeometricObject> planeBack{ new RayTracer::Rectangle(Point3D(-50, -50, -50), Vec3D(100, 0, 0), Vec3D(0, 100, 0)) };
    std::shared_ptr<Material> planeMat3{ new Matte() };
    std::dynamic_pointer_cast<Matte>(planeMat3)->SetKa(0.2);
    std::dynamic_pointer_cast<Matte>(planeMat3)->SetKd(0.9);
    std::dynamic_pointer_cast<Matte>(planeMat3)->SetCd(RGBColor(1, 1, 1));
    planeBack->SetMaterial(planeMat3);

    std::shared_ptr<GeometricObject> planeLeft{ new RayTracer::Rectangle(Point3D(-50, -50, 50), Vec3D(0, 0, -100), Vec3D(0, 100, 0)) };
    std::shared_ptr<Material> planeMat4{ new Matte() };
    std::dynamic_pointer_cast<Matte>(planeMat4)->SetKa(0.2);
    std::dynamic_pointer_cast<Matte>(planeMat4)->SetKd(0.9);
    std::dynamic_pointer_cast<Matte>(planeMat4)->SetCd(RGBColor(0.85490, 0.37647, 0.18431));
    planeLeft->SetMaterial(planeMat4);

    std::shared_ptr<GeometricObject> planeRight{ new RayTracer::Rectangle(Point3D(50, -50, 50), Vec3D(0, 100, 0), Vec3D(0, 0, -100)) };
    std::shared_ptr<Material> planeMat5{ new Matte() };
    std::dynamic_pointer_cast<Matte>(planeMat5)->SetKa(0.2);
    std::dynamic_pointer_cast<Matte>(planeMat5)->SetKd(0.9);
    std::dynamic_pointer_cast<Matte>(planeMat5)->SetCd(RGBColor(0.40784, 0.49412, 0.33725));
    planeRight->SetMaterial(planeMat5);

    //std::shared_ptr<GeometricObject> boxFrontOriginal{ new Box(Point3D(-15, -15, -15), Point3D(15, 15, 15)) };
    //std::shared_ptr<GeometricObject> boxFront{ new Instance(boxFrontOriginal) };
    //std::dynamic_pointer_cast<Instance>(boxFront)->RotateY(10 / PI_OVER_180);
    //std::dynamic_pointer_cast<Instance>(boxFront)->Translate(18, -35, 20);
    std::shared_ptr<Material> boxMat1{ new Matte() };
    std::dynamic_pointer_cast<Matte>(boxMat1)->SetKa(0.2);
    std::dynamic_pointer_cast<Matte>(boxMat1)->SetKd(0.9);
    std::dynamic_pointer_cast<Matte>(boxMat1)->SetCd(RGBColor(1, 1, 1));
    //boxFront->SetMaterial(boxMat1);

    std::shared_ptr<GeometricObject> sphere{ new Sphere(Point3D(18, -35, 20), 15) };
    std::shared_ptr<Material> sphereMat{ new Transparent() };
    std::dynamic_pointer_cast<Transparent>(sphereMat)->SetKa(0.0);
    std::dynamic_pointer_cast<Transparent>(sphereMat)->SetKd(0.0);
    std::dynamic_pointer_cast<Transparent>(sphereMat)->SetCr(RGBColor(0.7, 0.7, 1.0));
    std::dynamic_pointer_cast<Transparent>(sphereMat)->SetCt(RGBColor(0.7, 0.7, 1.0));
    std::dynamic_pointer_cast<Transparent>(sphereMat)->SetKr(0.1);
    std::dynamic_pointer_cast<Transparent>(sphereMat)->SetKt(0.9);
    std::dynamic_pointer_cast<Transparent>(sphereMat)->SetIOR(1.3);
    sphere->SetMaterial(sphereMat);

    std::shared_ptr<GeometricObject> boxBackOriginal{ new Box(Point3D(-15, -30, -15), Point3D(15, 30, 15)) };
    std::shared_ptr<GeometricObject> boxBack{ new Instance(boxBackOriginal) };
    std::dynamic_pointer_cast<Instance>(boxBack)->RotateY(25 * PI_OVER_180);
    std::dynamic_pointer_cast<Instance>(boxBack)->Translate(-17, -20, -15);
    std::shared_ptr<Material> boxMat2{ new Reflective() };
    std::dynamic_pointer_cast<Reflective>(boxMat2)->SetKa(0.1);
    std::dynamic_pointer_cast<Reflective>(boxMat2)->SetKd(0.1);
    std::dynamic_pointer_cast<Reflective>(boxMat2)->SetCd(RGBColor(1, 1, 1));
    std::dynamic_pointer_cast<Reflective>(boxMat2)->SetCr(RGBColor(1, 1, 1));
    std::dynamic_pointer_cast<Reflective>(boxMat2)->SetKr(0.8);
    boxBack->SetMaterial(boxMat2);

    std::shared_ptr<GeometricObject> comp{ new RayTracer::Grid() };

    std::dynamic_pointer_cast<RayTracer::Grid>(comp)->AddObject(lightplane1);
    std::dynamic_pointer_cast<RayTracer::Grid>(comp)->AddObject(planeBottom);
    std::dynamic_pointer_cast<RayTracer::Grid>(comp)->AddObject(planeTop);
    std::dynamic_pointer_cast<RayTracer::Grid>(comp)->AddObject(planeBack);
    std::dynamic_pointer_cast<RayTracer::Grid>(comp)->AddObject(planeLeft);
    std::dynamic_pointer_cast<RayTracer::Grid>(comp)->AddObject(planeRight);
    std::dynamic_pointer_cast<RayTracer::Grid>(comp)->AddObject(sphere);
    std::dynamic_pointer_cast<RayTracer::Grid>(comp)->AddObject(boxBack);

    std::dynamic_pointer_cast<RayTracer::Grid>(comp)->Setup();

    world->AddObject(comp);
}


/*
void MainComponent::setupWorld()
{
    auto r = 3.2;
    auto theta = 140.0 * PI_OVER_180;
    auto phi = 55 * PI_OVER_180;
    auto roll = 0.0 * PI_OVER_180;
    auto lookat = Vec3D(0.0, 0.2, 0.0);
    auto eyepoint = Vec3D(r * sin(theta) * sin(phi), r * cos(phi), r * cos(theta) * sin(phi)) + lookat;
    camera.reset(new PinholeCamera(eyepoint, lookat, Vec3D(sin(roll), cos(roll), 0.0), 1.0));
    //std::shared_ptr<Camera> cam1{ new PinholeCamera(eyepoint, lookat, Vec3D(sin(roll), cos(roll), 0.0), 1.0) };
    //std::shared_ptr<Camera> cam2{ new PinholeCamera(eyepoint, lookat, Vec3D(sin(roll), cos(roll), 0.0), 1.0) };
    //camera.reset(new StereoCamera(eyepoint, lookat, Vec3D(sin(roll), cos(roll), 0.0), cam1, cam2, 5, true));

    std::shared_ptr<Light> ambient{ new Ambient(1.0, RGBColor(1.0, 1.0, 1.0)) };
    world->SetAmbient(ambient);

    std::shared_ptr<GeometricObject> lightplane1{ new RayTracer::Rectangle(Point3D(-0.3, 1.0, 1.8), Vec3D(0, 0.5, 0), Vec3D(0.6, 0, 0)) };
    std::shared_ptr<Material> lightMat1{ new Emissive() };
    std::dynamic_pointer_cast<Emissive>(lightMat1)->SetLs(25);
    std::dynamic_pointer_cast<Emissive>(lightMat1)->SetCe(RGBColor(1.0, 1.0, 1.0));
    lightplane1->SetMaterial(lightMat1);
    std::shared_ptr<Light> arealight1{ new AreaLight(lightplane1, lightMat1) };
    world->AddLight(arealight1);

    std::shared_ptr<GeometricObject> lightplane2{ new RayTracer::Rectangle(Point3D(1.7, 1.0, -0.2), Vec3D(0, 0, 0.6), Vec3D(0, 0.5, 0)) };
    std::shared_ptr<Material> lightMat2{ new Emissive() };
    std::dynamic_pointer_cast<Emissive>(lightMat2)->SetLs(10);
    std::dynamic_pointer_cast<Emissive>(lightMat2)->SetCe(RGBColor(1.0, 1.0, 0.5));
    lightplane2->SetMaterial(lightMat2);
    std::shared_ptr<Light> arealight2{ new AreaLight(lightplane2, lightMat2) };
    world->AddLight(arealight2);

    std::shared_ptr<GeometricObject> lightplane3{ new RayTracer::Rectangle(Point3D(-1.7, 1.0, -0.2), Vec3D(0, 0.5, 0), Vec3D(0, 0, 0.6)) };
    std::shared_ptr<Material> lightMat3{ new Emissive() };
    std::dynamic_pointer_cast<Emissive>(lightMat3)->SetLs(10);
    std::dynamic_pointer_cast<Emissive>(lightMat3)->SetCe(RGBColor(0.5, 1.0, 1.0));
    lightplane3->SetMaterial(lightMat3);
    std::shared_ptr<Light> arealight3{ new AreaLight(lightplane3, lightMat3) };
    world->AddLight(arealight3);

    std::shared_ptr<LatticeNoise> cubicNoise{ new CubicNoise() };

    std::shared_ptr<GeometricObject> plane{ new RayTracer::Rectangle(Point3D(-3, 0, 3), Vec3D(6, 0, 0), Vec3D(0, 0, -6)) };
    std::shared_ptr<Texture> planeTex1Original{ new Checker3D() };
    std::dynamic_pointer_cast<Checker3D>(planeTex1Original)->setSize(0.3);
    std::dynamic_pointer_cast<Checker3D>(planeTex1Original)->setColor1(WHITE * 0.2);
    std::dynamic_pointer_cast<Checker3D>(planeTex1Original)->setColor2(WHITE * 0.6);
    std::shared_ptr<Material> planeMat1{ new Matte() };
    std::dynamic_pointer_cast<Matte>(planeMat1)->SetKa(0.2);
    std::dynamic_pointer_cast<Matte>(planeMat1)->SetKd(0.8);
    std::dynamic_pointer_cast<Matte>(planeMat1)->SetCd(planeTex1Original);
    plane->SetMaterial(planeMat1);

    std::shared_ptr<GeometricObject> cylinder{ new Cylinder(Point3D(-0.9, 0, 0.4), 0.3, 0.9) };
    std::shared_ptr<ColorRamp> rampWood{ new ColorRamp() };
    rampWood->buildFromImage(ImageFileFormat::loadFrom(File("D:\\testimg.png")), false);
    std::shared_ptr<Texture> cyTextureOriginal{ new RampNoiseTexture(cubicNoise, rampWood) };
    std::dynamic_pointer_cast<RampNoiseTexture>(cyTextureOriginal)->setType(NoiseTextureType::BROWNIAN);
    std::dynamic_pointer_cast<RampNoiseTexture>(cyTextureOriginal)->setAmount(0.01);
    std::dynamic_pointer_cast<RampNoiseTexture>(cyTextureOriginal)->setOctaves(6);
    std::dynamic_pointer_cast<RampNoiseTexture>(cyTextureOriginal)->setGain(0.5);
    std::dynamic_pointer_cast<RampNoiseTexture>(cyTextureOriginal)->setLacunarity(2);
    std::shared_ptr<Texture> woodTex1{ new TextureInstance(cyTextureOriginal) };
    std::dynamic_pointer_cast<TextureInstance>(woodTex1)->RotateX(-75 * PI_OVER_180);
    std::dynamic_pointer_cast<TextureInstance>(woodTex1)->Scale(0.25, 0.25, 0.25);
    std::shared_ptr<Material> cyMat1{ new Matte() };
    std::dynamic_pointer_cast<Matte>(cyMat1)->SetKa(0.2);
    std::dynamic_pointer_cast<Matte>(cyMat1)->SetKd(0.8);
    std::dynamic_pointer_cast<Matte>(cyMat1)->SetCd(woodTex1);
    cylinder->SetMaterial(cyMat1);

    std::shared_ptr<GeometricObject> cylinder2{ new Cylinder(Point3D(-0.9, 0, -0.5), 0.3, 0.9) };
    std::shared_ptr<Texture> cyTex1Original{ new NoiseTexture(cubicNoise) };
    std::dynamic_pointer_cast<NoiseTexture>(cyTex1Original)->setType(NoiseTextureType::FRACTAL_SUM);
    std::dynamic_pointer_cast<NoiseTexture>(cyTex1Original)->setOctaves(8);
    std::dynamic_pointer_cast<NoiseTexture>(cyTex1Original)->setColor(RGBColor(0.27, 0.55, 0.79));
    std::shared_ptr<Texture> cyTex1{ new TextureInstance(cyTex1Original) };
    std::dynamic_pointer_cast<TextureInstance>(cyTex1)->Scale(0.1, 0.1, 0.1);
    std::shared_ptr<Material> cyMat2{ new Matte() };
    std::dynamic_pointer_cast<Matte>(cyMat2)->SetKa(0.2);
    std::dynamic_pointer_cast<Matte>(cyMat2)->SetKd(0.8);
    std::dynamic_pointer_cast<Matte>(cyMat2)->SetCd(cyTex1);
    cylinder2->SetMaterial(cyMat2);

    std::shared_ptr<GeometricObject> sphere1{ new Sphere(Point3D(0.9, 0.35, 0.4), 0.35) };
    std::shared_ptr<Texture> sphereTex1Original{ new WrappedNoiseTexture(cubicNoise) };
    std::dynamic_pointer_cast<WrappedNoiseTexture>(sphereTex1Original)->setColor(RGBColor(0.2, 0.6, 0.3));
    std::dynamic_pointer_cast<WrappedNoiseTexture>(sphereTex1Original)->setExpansion(5);
    std::dynamic_pointer_cast<WrappedNoiseTexture>(sphereTex1Original)->setOctaves(6);
    std::dynamic_pointer_cast<WrappedNoiseTexture>(sphereTex1Original)->setGain(0.5);
    std::dynamic_pointer_cast<WrappedNoiseTexture>(sphereTex1Original)->setLacunarity(2);
    std::shared_ptr<Texture> sphereTex1{ new TextureInstance(sphereTex1Original) };
    std::dynamic_pointer_cast<TextureInstance>(sphereTex1)->Scale(0.5, 0.5, 0.5);
    std::shared_ptr<Material> sphereMat1{ new Matte() };
    std::dynamic_pointer_cast<Matte>(sphereMat1)->SetKa(0.2);
    std::dynamic_pointer_cast<Matte>(sphereMat1)->SetKd(0.8);
    std::dynamic_pointer_cast<Matte>(sphereMat1)->SetCd(sphereTex1);
    sphere1->SetMaterial(sphereMat1);

    std::shared_ptr<GeometricObject> sphere2{ new Sphere(Point3D(0.9, 0.35, -0.5), 0.35) };
    std::shared_ptr<ColorRamp> ramp{ new ColorRamp() };
    ramp->addColorPoint(0, WHITE);
    ramp->addColorPoint(0.7, RGBColor(0.04314, 0.17647, 0.29412));
    ramp->addColorPoint(1, BLACK);
    ramp->build();
    std::shared_ptr<Texture> sphereTex2Original{ new RampNoiseTexture(cubicNoise, ramp) };
    std::dynamic_pointer_cast<RampNoiseTexture>(sphereTex2Original)->setType(NoiseTextureType::BROWNIAN);
    std::dynamic_pointer_cast<RampNoiseTexture>(sphereTex2Original)->setAmount(3.5);
    std::dynamic_pointer_cast<RampNoiseTexture>(sphereTex2Original)->setOctaves(6);
    std::dynamic_pointer_cast<RampNoiseTexture>(sphereTex2Original)->setGain(0.5);
    std::dynamic_pointer_cast<RampNoiseTexture>(sphereTex2Original)->setLacunarity(2);
    std::shared_ptr<Texture> sphereTex2{ new TextureInstance(sphereTex2Original) };
    std::dynamic_pointer_cast<TextureInstance>(sphereTex2)->Scale(0.2, 0.2, 0.2);
    std::shared_ptr<Material> sphereMat2{ new Matte() };
    std::dynamic_pointer_cast<Matte>(sphereMat2)->SetKa(0.2);
    std::dynamic_pointer_cast<Matte>(sphereMat2)->SetKd(0.8);
    std::dynamic_pointer_cast<Matte>(sphereMat2)->SetCd(sphereTex2);
    sphere2->SetMaterial(sphereMat2);

    std::shared_ptr<GeometricObject> box{ new Box(Point3D(-1, -1, -1), Point3D(1, 1, 1)) };
    std::shared_ptr<GeometricObject> csgsphere1{ new Sphere(Point3D(0, 0, 0), 1.3) };
    std::shared_ptr<GeometricObject> cy1{ new Cylinder(Point3D(0, -2, 0), 0.65, 4) };
    std::shared_ptr<GeometricObject> cy2{ new Instance(cy1) };
    std::dynamic_pointer_cast<Instance>(cy2)->RotateX(90 * PI_OVER_180);
    std::shared_ptr<GeometricObject> cy3{ new Instance(cy1) };
    std::dynamic_pointer_cast<Instance>(cy3)->RotateZ(90 * PI_OVER_180);

    std::shared_ptr<Material> cyMat{ new Matte() };
    std::dynamic_pointer_cast<Matte>(cyMat)->SetKa(0.3);
    std::dynamic_pointer_cast<Matte>(cyMat)->SetKd(0.7);
    std::dynamic_pointer_cast<Matte>(cyMat)->SetCd(RED);
    cy1->SetMaterial(cyMat);
    cy2->SetMaterial(cyMat);
    cy3->SetMaterial(cyMat);
    std::shared_ptr<Material> boxMat{ new Matte() };
    std::dynamic_pointer_cast<Matte>(boxMat)->SetKa(0.3);
    std::dynamic_pointer_cast<Matte>(boxMat)->SetKd(0.7);
    std::dynamic_pointer_cast<Matte>(boxMat)->SetCd(GREEN);
    box->SetMaterial(boxMat);
    std::shared_ptr<Material> sphereMat{ new Matte() };
    std::dynamic_pointer_cast<Matte>(sphereMat)->SetKa(0.3);
    std::dynamic_pointer_cast<Matte>(sphereMat)->SetKd(0.7);
    std::dynamic_pointer_cast<Matte>(sphereMat)->SetCd(BLUE);
    csgsphere1->SetMaterial(sphereMat);

    std::shared_ptr<GeometricObject> csg{ new CSG() };
    std::dynamic_pointer_cast<CSG>(csg)->addObject(box);
    std::dynamic_pointer_cast<CSG>(csg)->addObject(csgsphere1);
    std::dynamic_pointer_cast<CSG>(csg)->addOperation(CSG::OpType::INTERSECTION);
    std::dynamic_pointer_cast<CSG>(csg)->addObject(cy1);
    std::dynamic_pointer_cast<CSG>(csg)->addObject(cy2);
    std::dynamic_pointer_cast<CSG>(csg)->addOperation(CSG::OpType::UNION);
    std::dynamic_pointer_cast<CSG>(csg)->addObject(cy3);
    std::dynamic_pointer_cast<CSG>(csg)->addOperation(CSG::OpType::UNION);
    std::dynamic_pointer_cast<CSG>(csg)->addOperation(CSG::OpType::DIFFERENCE);
    std::dynamic_pointer_cast<CSG>(csg)->build();

    std::shared_ptr<GeometricObject> csgins{ new Instance(csg) };
    std::dynamic_pointer_cast<Instance>(csgins)->Scale(0.35, 0.35, 0.35);
    std::dynamic_pointer_cast<Instance>(csgins)->Translate(0, 0.35, 0.4);
    std::shared_ptr<Material> csgmat1{ new Reflective() };
    std::dynamic_pointer_cast<Reflective>(csgmat1)->SetKa(0.1);
    std::dynamic_pointer_cast<Reflective>(csgmat1)->SetKd(0.1);
    std::dynamic_pointer_cast<Reflective>(csgmat1)->SetCd(RGBColor(0.8, 0.6, 1.0));
    std::dynamic_pointer_cast<Reflective>(csgmat1)->SetCr(RGBColor(0.8, 0.6, 1.0));
    std::dynamic_pointer_cast<Reflective>(csgmat1)->SetKr(0.8);
    csgins->SetMaterial(csgmat1);

    std::shared_ptr<GeometricObject> csgins2{ new Instance(csg) };
    std::dynamic_pointer_cast<Instance>(csgins2)->Scale(0.35, 0.35, 0.35);
    std::dynamic_pointer_cast<Instance>(csgins2)->Translate(0, 0.35, -0.5);
    std::shared_ptr<Material> csgmat2{ new Transparent() };
    std::dynamic_pointer_cast<Transparent>(csgmat2)->SetKa(0.05);
    std::dynamic_pointer_cast<Transparent>(csgmat2)->SetKd(0.05);
    std::dynamic_pointer_cast<Transparent>(csgmat2)->SetCd(RGBColor(1.0, 0.4, 0.8));
    std::dynamic_pointer_cast<Transparent>(csgmat2)->SetCr(RGBColor(1.0, 0.8, 0.8));
    std::dynamic_pointer_cast<Transparent>(csgmat2)->SetCt(RGBColor(1.0, 0.8, 0.8));
    std::dynamic_pointer_cast<Transparent>(csgmat2)->SetKr(0.5);
    std::dynamic_pointer_cast<Transparent>(csgmat2)->SetKt(1.0);
    std::dynamic_pointer_cast<Transparent>(csgmat2)->SetIOR(1.2);
    csgins2->SetMaterial(csgmat2);

    std::shared_ptr<GeometricObject> comp{ new RayTracer::Grid() };

    std::dynamic_pointer_cast<RayTracer::Grid>(comp)->AddObject(plane);
    std::dynamic_pointer_cast<RayTracer::Grid>(comp)->AddObject(lightplane1);
    std::dynamic_pointer_cast<RayTracer::Grid>(comp)->AddObject(lightplane2);
    std::dynamic_pointer_cast<RayTracer::Grid>(comp)->AddObject(lightplane3);
    std::dynamic_pointer_cast<RayTracer::Grid>(comp)->AddObject(csgins);
    std::dynamic_pointer_cast<RayTracer::Grid>(comp)->AddObject(csgins2);
    std::dynamic_pointer_cast<RayTracer::Grid>(comp)->AddObject(cylinder);
    std::dynamic_pointer_cast<RayTracer::Grid>(comp)->AddObject(cylinder2);
    std::dynamic_pointer_cast<RayTracer::Grid>(comp)->AddObject(sphere1);
    std::dynamic_pointer_cast<RayTracer::Grid>(comp)->AddObject(sphere2);

    std::dynamic_pointer_cast<RayTracer::Grid>(comp)->Setup();

    world->AddObject(comp);
}
*/

/*
void MainComponent::setupWorld()
{
    auto r = 2.5;
    auto theta = 160 * PI_OVER_180;
    auto phi = 70 * PI_OVER_180;
    auto roll = 0.0 * PI_OVER_180;
    auto lookat = Vec3D(0.0, 0.3, 0.0);
    auto eyepoint = Vec3D(r * sin(theta * PI_OVER_180) * sin(phi), r * cos(phi), r * cos(theta * PI_OVER_180) * sin(phi)) + lookat;
    // theta curve ((exp(-(x/40)^2)^3+exp(-(((x-365/3)/40))^2)^3+exp(-(((x-365/1.5)/40))^2)^3+exp(-(((x-365)/40))^2)^3) + 0.5)/305.299*365
    //camera.reset(new ThinLensCamera(eyepoint, lookat, Vec3D(sin(roll), cos(roll), 0.0), 1.0, 1.3, 0.05));
    std::shared_ptr<Camera> cam1{ new ThinLensCamera(eyepoint, lookat, Vec3D(sin(roll), cos(roll), 0.0), 1.0, 1.3, 0.05) };
    std::shared_ptr<Camera> cam2{ new ThinLensCamera(eyepoint, lookat, Vec3D(sin(roll), cos(roll), 0.0), 1.0, 1.3, 0.05) };
    camera.reset(new StereoCamera(eyepoint, lookat, Vec3D(sin(roll), cos(roll), 0.0), cam1, cam2, 5, true));

    std::shared_ptr<Light> ambient{ new Ambient(0.5, RGBColor(1.0, 1.0, 1.0)) };
    world->SetAmbient(ambient);

    FP_TYPE degInc = 365 / 8.0;
    FP_TYPE lightR = 1.7;
    std::vector<std::shared_ptr<GeometricObject>> lights;
    for (int deg = 0; deg < 8; deg++)
    {
        auto radian = deg * degInc * PI_OVER_180;
        auto center = Point3D(lightR * cos(radian), 2, lightR * sin(radian));
        std::shared_ptr<GeometricObject> lightdisk{ new Disk(center, 0.3, Vec3D(0, -1, 0) * 0.6 + (ZERO - center).normalised() * 0.4) };
        std::shared_ptr<Material> lightMat{ new Emissive() };
        std::dynamic_pointer_cast<Emissive>(lightMat)->SetLs(4 * abs(cos(2 * (deg * 365 / 8.0) * PI_OVER_180)));
        auto color = Colour::fromHSV(radian / TWO_PI, 0.9, 1.0, 1.0);
        std::dynamic_pointer_cast<Emissive>(lightMat)->SetCe(RGBColor(color.getRed() / 255.0, color.getGreen() / 255.0, color.getBlue() / 255.0));
        lightdisk->SetMaterial(lightMat);
        std::shared_ptr<Light> arealight{ new AreaLight(lightdisk, lightMat) };
        world->AddLight(arealight);
        lights.push_back(lightdisk);
    }

    std::shared_ptr<LatticeNoise> cubicNoise{ new CubicNoise() };

    std::shared_ptr<GeometricObject> plane{ new RayTracer::Rectangle(Point3D(-3, 0, 3), Vec3D(6, 0, 0), Vec3D(0, 0, -6)) };
    std::shared_ptr<Texture> planeTex1Original{ new Checker3D() };
    std::dynamic_pointer_cast<Checker3D>(planeTex1Original)->setSize(0.3);
    std::dynamic_pointer_cast<Checker3D>(planeTex1Original)->setColor1(WHITE * 0.2);
    std::dynamic_pointer_cast<Checker3D>(planeTex1Original)->setColor2(WHITE * 0.6);
    std::shared_ptr<Material> planeMat1{ new Matte() };
    std::dynamic_pointer_cast<Matte>(planeMat1)->SetKa(0.2);
    std::dynamic_pointer_cast<Matte>(planeMat1)->SetKd(0.8);
    std::dynamic_pointer_cast<Matte>(planeMat1)->SetCd(planeTex1Original);
    //std::dynamic_pointer_cast<Matte>(planeMat1)->SetCd(WHITE * 0.5);
    //std::shared_ptr<Material> planeMat1{ new GlossyReflector() };
    //std::dynamic_pointer_cast<GlossyReflector>(planeMat1)->SetKa(0.1);
    //std::dynamic_pointer_cast<GlossyReflector>(planeMat1)->SetKd(0.4);
    //std::dynamic_pointer_cast<GlossyReflector>(planeMat1)->SetKs(0.4);
    //std::dynamic_pointer_cast<GlossyReflector>(planeMat1)->SetCd(RGBColor(1.0, 1.0, 1.0));
    //std::dynamic_pointer_cast<GlossyReflector>(planeMat1)->SetCs(RGBColor(1.0, 1.0, 1.0));
    //std::dynamic_pointer_cast<GlossyReflector>(planeMat1)->SetE(500.0);
    plane->SetMaterial(planeMat1);

    std::shared_ptr<GeometricObject> platCylinder1{ new Cylinder(Point3D(0.3, -0.05, 0), 0.06, 0.4) };
    std::shared_ptr<GeometricObject> platCylinder2{ new Cylinder(Point3D(-0.3, -0.05, 0), 0.06, 0.4) };
    std::shared_ptr<GeometricObject> platCylinder3{ new Cylinder(Point3D(0, -0.05, 0.3), 0.06, 0.4) };
    std::shared_ptr<GeometricObject> platCylinder4{ new Cylinder(Point3D(0, -0.05, -0.3), 0.06, 0.4) };
    std::shared_ptr<GeometricObject> platCylinder5{ new Cylinder(Point3D(0, -0.05, 0), 0.38, 0.4) };
    std::shared_ptr<GeometricObject> platBox{ new Box(Point3D(-0.3, 0, -0.3), Point3D(0.3, 0.2, 0.3)) };
    std::shared_ptr<GeometricObject> platform{ new CSG() };
    std::dynamic_pointer_cast<CSG>(platform)->addObject(platBox);
    std::dynamic_pointer_cast<CSG>(platform)->addObject(platCylinder5);
    std::dynamic_pointer_cast<CSG>(platform)->addOperation(CSG::OpType::INTERSECTION);
    std::dynamic_pointer_cast<CSG>(platform)->addObject(platCylinder1);
    std::dynamic_pointer_cast<CSG>(platform)->addOperation(CSG::OpType::DIFFERENCE);
    std::dynamic_pointer_cast<CSG>(platform)->addObject(platCylinder2);
    std::dynamic_pointer_cast<CSG>(platform)->addOperation(CSG::OpType::DIFFERENCE);
    std::dynamic_pointer_cast<CSG>(platform)->addObject(platCylinder3);
    std::dynamic_pointer_cast<CSG>(platform)->addOperation(CSG::OpType::DIFFERENCE);
    std::dynamic_pointer_cast<CSG>(platform)->addObject(platCylinder4);
    std::dynamic_pointer_cast<CSG>(platform)->addOperation(CSG::OpType::DIFFERENCE);
    std::dynamic_pointer_cast<CSG>(platform)->build();

    std::shared_ptr<GeometricObject> platform1{ new Instance(platform) };
    std::dynamic_pointer_cast<Instance>(platform1)->Scale(1.33, 1.33, 1.33);
    std::dynamic_pointer_cast<Instance>(platform1)->Translate(0.65 * 1.33, 0, 0);
    std::shared_ptr<ColorRamp> rampWood1{ new ColorRamp() };
    rampWood1->buildFromImage(ImageFileFormat::loadFrom(BinaryData::wood1_png, BinaryData::wood1_pngSize));
    std::shared_ptr<Texture> woodTexture1{ new RampNoiseTexture(cubicNoise, rampWood1) };
    std::dynamic_pointer_cast<RampNoiseTexture>(woodTexture1)->setType(NoiseTextureType::BROWNIAN);
    std::dynamic_pointer_cast<RampNoiseTexture>(woodTexture1)->setAmount(0.02);
    std::dynamic_pointer_cast<RampNoiseTexture>(woodTexture1)->setOctaves(6);
    std::dynamic_pointer_cast<RampNoiseTexture>(woodTexture1)->setGain(0.5);
    std::dynamic_pointer_cast<RampNoiseTexture>(woodTexture1)->setLacunarity(2);
    std::shared_ptr<Texture> woodTex1{ new TextureInstance(woodTexture1) };
    std::dynamic_pointer_cast<TextureInstance>(woodTex1)->RotateX(-73 * PI_OVER_180);
    std::dynamic_pointer_cast<TextureInstance>(woodTex1)->RotateY(35 * PI_OVER_180);
    std::dynamic_pointer_cast<TextureInstance>(woodTex1)->Scale(0.3, 0.25, 0.4);
    std::shared_ptr<Material> platMat1{ new Matte() };
    std::dynamic_pointer_cast<Matte>(platMat1)->SetKa(0.2);
    std::dynamic_pointer_cast<Matte>(platMat1)->SetKd(0.8);
    std::dynamic_pointer_cast<Matte>(platMat1)->SetCd(woodTex1);
    //std::dynamic_pointer_cast<Matte>(platMat1)->SetCd(WHITE * 0.5);
    platform1->SetMaterial(platMat1);

    std::shared_ptr<GeometricObject> platform2{ new Instance(platform) };
    std::dynamic_pointer_cast<Instance>(platform2)->Scale(1.33, 1.33, 1.33);
    std::dynamic_pointer_cast<Instance>(platform2)->Translate(-0.65 * 1.33, 0, 0);
    std::shared_ptr<ColorRamp> rampWood2{ new ColorRamp() };
    rampWood2->buildFromImage(ImageFileFormat::loadFrom(BinaryData::wood2_png, BinaryData::wood2_pngSize));
    std::shared_ptr<Texture> woodTexture2{ new RampNoiseTexture(cubicNoise, rampWood2) };
    std::dynamic_pointer_cast<RampNoiseTexture>(woodTexture2)->setType(NoiseTextureType::BROWNIAN);
    std::dynamic_pointer_cast<RampNoiseTexture>(woodTexture2)->setAmount(0.021);
    std::dynamic_pointer_cast<RampNoiseTexture>(woodTexture2)->setOctaves(6);
    std::dynamic_pointer_cast<RampNoiseTexture>(woodTexture2)->setGain(0.5);
    std::dynamic_pointer_cast<RampNoiseTexture>(woodTexture2)->setLacunarity(2);
    std::shared_ptr<Texture> woodTex2{ new TextureInstance(woodTexture2) };
    std::dynamic_pointer_cast<TextureInstance>(woodTex2)->RotateX(-64 * PI_OVER_180);
    std::dynamic_pointer_cast<TextureInstance>(woodTex2)->RotateY(83 * PI_OVER_180);
    std::dynamic_pointer_cast<TextureInstance>(woodTex2)->Scale(0.35, 0.254, 0.42);
    std::shared_ptr<Material> platMat2{ new Matte() };
    std::dynamic_pointer_cast<Matte>(platMat2)->SetKa(0.2);
    std::dynamic_pointer_cast<Matte>(platMat2)->SetKd(0.8);
    std::dynamic_pointer_cast<Matte>(platMat2)->SetCd(woodTex2);
    //std::dynamic_pointer_cast<Matte>(platMat2)->SetCd(WHITE * 0.5);
    platform2->SetMaterial(platMat2);

    std::shared_ptr<GeometricObject> platform3{ new Instance(platform) };
    std::dynamic_pointer_cast<Instance>(platform3)->Scale(1.33, 1.33, 1.33);
    std::dynamic_pointer_cast<Instance>(platform3)->Translate(0, 0, 0.65 * 1.33);
    std::shared_ptr<ColorRamp> rampWood3{ new ColorRamp() };
    rampWood3->buildFromImage(ImageFileFormat::loadFrom(BinaryData::wood3_png, BinaryData::wood3_pngSize));
    std::shared_ptr<Texture> woodTexture3{ new RampNoiseTexture(cubicNoise, rampWood3) };
    std::dynamic_pointer_cast<RampNoiseTexture>(woodTexture3)->setType(NoiseTextureType::BROWNIAN);
    std::dynamic_pointer_cast<RampNoiseTexture>(woodTexture3)->setAmount(0.019);
    std::dynamic_pointer_cast<RampNoiseTexture>(woodTexture3)->setOctaves(6);
    std::dynamic_pointer_cast<RampNoiseTexture>(woodTexture3)->setGain(0.5);
    std::dynamic_pointer_cast<RampNoiseTexture>(woodTexture3)->setLacunarity(2);
    std::shared_ptr<Texture> woodTex3{ new TextureInstance(woodTexture3) };
    std::dynamic_pointer_cast<TextureInstance>(woodTex3)->RotateX(-71 * PI_OVER_180);
    std::dynamic_pointer_cast<TextureInstance>(woodTex3)->RotateY(53 * PI_OVER_180);
    std::dynamic_pointer_cast<TextureInstance>(woodTex3)->Scale(0.38, 0.254, 0.43);
    std::shared_ptr<Material> platMat3{ new Matte() };
    std::dynamic_pointer_cast<Matte>(platMat3)->SetKa(0.2);
    std::dynamic_pointer_cast<Matte>(platMat3)->SetKd(0.8);
    std::dynamic_pointer_cast<Matte>(platMat3)->SetCd(woodTex3);
    //std::dynamic_pointer_cast<Matte>(platMat3)->SetCd(WHITE * 0.5);
    platform3->SetMaterial(platMat3);

    std::shared_ptr<GeometricObject> platform4{ new Instance(platform) };
    std::dynamic_pointer_cast<Instance>(platform4)->Scale(1.33, 1.33, 1.33);
    std::dynamic_pointer_cast<Instance>(platform4)->Translate(0, 0, -0.65 * 1.33);
    std::shared_ptr<ColorRamp> rampWood4{ new ColorRamp() };
    rampWood4->buildFromImage(ImageFileFormat::loadFrom(BinaryData::wood4_png, BinaryData::wood4_pngSize));
    std::shared_ptr<Texture> woodTexture4{ new RampNoiseTexture(cubicNoise, rampWood4) };
    std::dynamic_pointer_cast<RampNoiseTexture>(woodTexture4)->setType(NoiseTextureType::BROWNIAN);
    std::dynamic_pointer_cast<RampNoiseTexture>(woodTexture4)->setAmount(0.02);
    std::dynamic_pointer_cast<RampNoiseTexture>(woodTexture4)->setOctaves(6);
    std::dynamic_pointer_cast<RampNoiseTexture>(woodTexture4)->setGain(0.5);
    std::dynamic_pointer_cast<RampNoiseTexture>(woodTexture4)->setLacunarity(2);
    std::shared_ptr<Texture> woodTex4{ new TextureInstance(woodTexture4) };
    std::dynamic_pointer_cast<TextureInstance>(woodTex4)->RotateX(-69 * PI_OVER_180);
    std::dynamic_pointer_cast<TextureInstance>(woodTex4)->RotateY(42 * PI_OVER_180);
    std::dynamic_pointer_cast<TextureInstance>(woodTex4)->Scale(0.35, 0.253, 0.41);
    std::shared_ptr<Material> platMat4{ new Matte() };
    std::dynamic_pointer_cast<Matte>(platMat4)->SetKa(0.2);
    std::dynamic_pointer_cast<Matte>(platMat4)->SetKd(0.8);
    std::dynamic_pointer_cast<Matte>(platMat4)->SetCd(woodTex4);
    //std::dynamic_pointer_cast<Matte>(platMat4)->SetCd(WHITE * 0.5);
    platform4->SetMaterial(platMat4);


    std::shared_ptr<GeometricObject> csg1box{ new Box(Point3D(-1, -1, -1), Point3D(1, 1, 1)) };
    std::shared_ptr<GeometricObject> csg1sphere{ new Sphere(Point3D(0, 0, 0), 1.3) };
    std::shared_ptr<GeometricObject> csg1cy1{ new Cylinder(Point3D(0, -2, 0), 0.65, 4) };
    std::shared_ptr<GeometricObject> csg1cy2{ new Instance(csg1cy1) };
    std::dynamic_pointer_cast<Instance>(csg1cy2)->RotateX(90 * PI_OVER_180);
    std::shared_ptr<GeometricObject> csg1cy3{ new Instance(csg1cy1) };
    std::dynamic_pointer_cast<Instance>(csg1cy3)->RotateZ(90 * PI_OVER_180);
    std::shared_ptr<GeometricObject> csg1{ new CSG() };
    std::dynamic_pointer_cast<CSG>(csg1)->addObject(csg1box);
    std::dynamic_pointer_cast<CSG>(csg1)->addObject(csg1sphere);
    std::dynamic_pointer_cast<CSG>(csg1)->addOperation(CSG::OpType::INTERSECTION);
    std::dynamic_pointer_cast<CSG>(csg1)->addObject(csg1cy1);
    std::dynamic_pointer_cast<CSG>(csg1)->addObject(csg1cy2);
    std::dynamic_pointer_cast<CSG>(csg1)->addOperation(CSG::OpType::UNION);
    std::dynamic_pointer_cast<CSG>(csg1)->addObject(csg1cy3);
    std::dynamic_pointer_cast<CSG>(csg1)->addOperation(CSG::OpType::UNION);
    std::dynamic_pointer_cast<CSG>(csg1)->addOperation(CSG::OpType::DIFFERENCE);
    std::dynamic_pointer_cast<CSG>(csg1)->build();
    std::shared_ptr<GeometricObject> csg1ins{ new Instance(csg1) };
    std::dynamic_pointer_cast<Instance>(csg1ins)->Scale(0.35, 0.35, 0.35);
    std::dynamic_pointer_cast<Instance>(csg1ins)->Translate(0, 0.35 + 0.2 * 1.33, -0.65 * 1.33);
    std::shared_ptr<Material> csgmat1{ new Transparent() };
    std::dynamic_pointer_cast<Transparent>(csgmat1)->SetKa(0.05);
    std::dynamic_pointer_cast<Transparent>(csgmat1)->SetKd(0.05);
    std::dynamic_pointer_cast<Transparent>(csgmat1)->SetCd(RGBColor(1.0, 0.4, 0.8));
    std::dynamic_pointer_cast<Transparent>(csgmat1)->SetCr(RGBColor(1.0, 0.8, 0.8));
    std::dynamic_pointer_cast<Transparent>(csgmat1)->SetCt(RGBColor(1.0, 0.8, 0.8));
    std::dynamic_pointer_cast<Transparent>(csgmat1)->SetKr(0.4);
    std::dynamic_pointer_cast<Transparent>(csgmat1)->SetKt(1.0);
    std::dynamic_pointer_cast<Transparent>(csgmat1)->SetIOR(1.2);
    //std::shared_ptr<Material> csgmat1{ new Matte() };
    //std::dynamic_pointer_cast<Matte>(csgmat1)->SetKa(0.2);
    //std::dynamic_pointer_cast<Matte>(csgmat1)->SetKd(0.8);
    //std::dynamic_pointer_cast<Matte>(csgmat1)->SetCd(WHITE * 0.5);
    csg1ins->SetMaterial(csgmat1);


    std::shared_ptr<GeometricObject> csg2box{ new Box(Point3D(-1, -1, -1), Point3D(1, 1, 1)) };
    std::shared_ptr<GeometricObject> csg2torus1{ new Torus(1.2, 0.4) };
    std::shared_ptr<GeometricObject> csg2torus2{ new Instance(csg2torus1) };
    std::dynamic_pointer_cast<Instance>(csg2torus2)->RotateX(90 * PI_OVER_180);
    std::shared_ptr<GeometricObject> csg2torus3{ new Instance(csg2torus1) };
    std::dynamic_pointer_cast<Instance>(csg2torus3)->RotateZ(90 * PI_OVER_180);
    std::shared_ptr<GeometricObject> csg2{ new CSG() };
    std::dynamic_pointer_cast<CSG>(csg2)->addObject(csg2box);
    std::dynamic_pointer_cast<CSG>(csg2)->addObject(csg2torus1);
    std::dynamic_pointer_cast<CSG>(csg2)->addOperation(CSG::OpType::DIFFERENCE);
    std::dynamic_pointer_cast<CSG>(csg2)->addObject(csg2torus2);
    std::dynamic_pointer_cast<CSG>(csg2)->addOperation(CSG::OpType::DIFFERENCE);
    std::dynamic_pointer_cast<CSG>(csg2)->addObject(csg2torus3);
    std::dynamic_pointer_cast<CSG>(csg2)->addOperation(CSG::OpType::DIFFERENCE);
    std::dynamic_pointer_cast<CSG>(csg2)->build();
    std::shared_ptr<GeometricObject> csg2ins{ new Instance(csg2) };
    std::dynamic_pointer_cast<Instance>(csg2ins)->Scale(0.35, 0.35, 0.35);
    std::dynamic_pointer_cast<Instance>(csg2ins)->Translate(0, 0.35 + 0.2 * 1.33, 0.65 * 1.33);
    std::shared_ptr<Material> csgmat2{ new GlossyReflector() };
    std::dynamic_pointer_cast<GlossyReflector>(csgmat2)->SetKa(0.1);
    std::dynamic_pointer_cast<GlossyReflector>(csgmat2)->SetKd(0.4);
    std::dynamic_pointer_cast<GlossyReflector>(csgmat2)->SetKs(0.4);
    std::dynamic_pointer_cast<GlossyReflector>(csgmat2)->SetCd(RGBColor(0.4, 0.6, 1.0));
    std::dynamic_pointer_cast<GlossyReflector>(csgmat2)->SetCs(RGBColor(0.4, 0.6, 1.0));
    std::dynamic_pointer_cast<GlossyReflector>(csgmat2)->SetE(700.0);
    //std::shared_ptr<Material> csgmat2{ new Matte() };
    //std::dynamic_pointer_cast<Matte>(csgmat2)->SetKa(0.2);
    //std::dynamic_pointer_cast<Matte>(csgmat2)->SetKd(0.8);
    //std::dynamic_pointer_cast<Matte>(csgmat2)->SetCd(WHITE * 0.5);
    csg2ins->SetMaterial(csgmat2);

    StringArray mesh1Content;
    mesh1Content.addLines((String(BinaryData::bunny_obj, BinaryData::bunny_objSize)));
    std::shared_ptr<GeometricObject> mesh1 = std::make_shared<Mesh>(OBJParser().parse(mesh1Content));
    std::shared_ptr<GeometricObject> mesh1ins{ new Instance(mesh1) };
    std::dynamic_pointer_cast<Instance>(mesh1ins)->Scale(0.7, 0.7, 0.7);
    std::dynamic_pointer_cast<Instance>(mesh1ins)->RotateX(-145 * PI_OVER_180);
    std::dynamic_pointer_cast<Instance>(mesh1ins)->RotateZ(8 * PI_OVER_180);
    std::dynamic_pointer_cast<Instance>(mesh1ins)->RotateY(-145 * PI_OVER_180);
    std::dynamic_pointer_cast<Instance>(mesh1ins)->Translate(0.65 * 1.33, 0.28 + 0.2 * 1.33, 0);
    std::shared_ptr<Material> meshMat1{ new Reflective() };
    std::dynamic_pointer_cast<Reflective>(meshMat1)->SetKa(0.1);
    std::dynamic_pointer_cast<Reflective>(meshMat1)->SetKd(0.1);
    std::dynamic_pointer_cast<Reflective>(meshMat1)->SetCd(RGBColor(0.9, 0.7, 0.3));
    std::dynamic_pointer_cast<Reflective>(meshMat1)->SetCr(RGBColor(1.0, 0.7, 0.3));
    std::dynamic_pointer_cast<Reflective>(meshMat1)->SetKr(0.8);
    //std::shared_ptr<Material> meshMat1{ new Matte() };
    //std::dynamic_pointer_cast<Matte>(meshMat1)->SetKa(0.7);
    //std::dynamic_pointer_cast<Matte>(meshMat1)->SetKd(0.8);
    //std::dynamic_pointer_cast<Matte>(meshMat1)->SetCd(WHITE * 0.5);
    mesh1ins->SetMaterial(meshMat1);

    StringArray mesh2Content;
    mesh2Content.addLines((String(BinaryData::dragon_obj, BinaryData::dragon_objSize)));
    std::shared_ptr<GeometricObject> mesh2 = std::make_shared<Mesh>(OBJParser().parse(mesh2Content));
    std::shared_ptr<GeometricObject> mesh2ins{ new Instance(mesh2) };
    std::dynamic_pointer_cast<Instance>(mesh2ins)->Scale(0.65, 0.65, 0.65);
    std::dynamic_pointer_cast<Instance>(mesh2ins)->RotateX(-12 * PI_OVER_180);
    std::dynamic_pointer_cast<Instance>(mesh2ins)->RotateZ(6 * PI_OVER_180);
    std::dynamic_pointer_cast<Instance>(mesh2ins)->RotateY(-50 * PI_OVER_180);
    std::dynamic_pointer_cast<Instance>(mesh2ins)->Translate(-0.63 * 1.33, 0.3 + 0.2 * 1.33, 0);
    std::shared_ptr<Material> meshMat2{ new Transparent() };
    std::dynamic_pointer_cast<Transparent>(meshMat2)->SetKa(0.05);
    std::dynamic_pointer_cast<Transparent>(meshMat2)->SetKd(0.05);
    std::dynamic_pointer_cast<Transparent>(meshMat2)->SetCd(RGBColor(0.4, 0.7, 1.0));
    std::dynamic_pointer_cast<Transparent>(meshMat2)->SetCr(RGBColor(0.4, 0.7, 1.0));
    std::dynamic_pointer_cast<Transparent>(meshMat2)->SetCt(RGBColor(0.4, 0.7, 1.0));
    std::dynamic_pointer_cast<Transparent>(meshMat2)->SetKr(0.3);
    std::dynamic_pointer_cast<Transparent>(meshMat2)->SetKt(1.4);
    std::dynamic_pointer_cast<Transparent>(meshMat2)->SetIOR(1.3);
    //std::shared_ptr<Material> meshMat2{ new Matte() };
    //std::dynamic_pointer_cast<Matte>(meshMat2)->SetKa(0.7);
    //std::dynamic_pointer_cast<Matte>(meshMat2)->SetKd(0.8);
    //std::dynamic_pointer_cast<Matte>(meshMat2)->SetCd(WHITE * 0.5);
    mesh2ins->SetMaterial(meshMat2);


    std::shared_ptr<GeometricObject> comp{ new RayTracer::Grid() };

    std::dynamic_pointer_cast<RayTracer::Grid>(comp)->AddObject(plane);
    for (auto light : lights)
        std::dynamic_pointer_cast<RayTracer::Grid>(comp)->AddObject(light);
    std::dynamic_pointer_cast<RayTracer::Grid>(comp)->AddObject(platform1);
    std::dynamic_pointer_cast<RayTracer::Grid>(comp)->AddObject(platform2);
    std::dynamic_pointer_cast<RayTracer::Grid>(comp)->AddObject(platform3);
    std::dynamic_pointer_cast<RayTracer::Grid>(comp)->AddObject(platform4);
    std::dynamic_pointer_cast<RayTracer::Grid>(comp)->AddObject(csg1ins);
    std::dynamic_pointer_cast<RayTracer::Grid>(comp)->AddObject(csg2ins);
    std::dynamic_pointer_cast<RayTracer::Grid>(comp)->AddObject(mesh1ins);
    std::dynamic_pointer_cast<RayTracer::Grid>(comp)->AddObject(mesh2ins);

    std::dynamic_pointer_cast<RayTracer::Grid>(comp)->Setup();

    world->AddObject(comp);
}
*/

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
			setSize(std::max(100, viewPlane->isStereo ? 2 * vpWidth : vpWidth), std::max(100, viewPlane->Height) + progressBar->getHeight());
#else
			setSize(std::max(100, viewPlane->isStereo ? 2 * vpWidth : vpWidth), std::max(100, viewPlane->Height) + progressBar->getHeight() +
                LookAndFeel::getDefaultLookAndFeel().getDefaultMenuBarHeight());
#endif
			progressBar->setVisible(true);
			repaint();
			renderer.Render(progress);
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
		setSize(std::max(100, viewPlane->isStereo ? 2 * vpWidth : vpWidth), std::max(100, viewPlane->Height));
#else
		setSize(std::max(100, viewPlane->isStereo ? 2 * vpWidth : vpWidth), std::max(100, viewPlane->Height) + 
            LookAndFeel::getDefaultLookAndFeel().getDefaultMenuBarHeight());
#endif
		repaint();
	}
}
