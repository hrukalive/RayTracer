namespace RayTracer

open System
open Eto.Forms
open Eto.Drawing
open RayTracer
open RayTracer.BaseTypes
open RayTracer.BaseFunctions
open RayTracer.GeometricObjects
open RayTracer.Material
open RayTracer.Tracer
open RayTracer.World
open RayTracer.Camera
open RayTracer.Light

type MainForm () as this =
    inherit Form()
    do
        this.InitializeComponent()

    member this.InitializeComponent() =
        base.Title <- "Ray Tracer"
        let (width, height) = (128 * 5, 72 * 4)

        let hitableList : list<GeometricObject* IMaterial> = [
            (Sphere(Vec3(0.0, 0.0, -2.0), 0.5) :> GeometricObject, Lambertian(Vec3(0.0, 1.0, 0.0), 1.0) :> IMaterial)
            (Sphere(Vec3(-1.0, 0.0, -1.0), 0.5) :> GeometricObject, Lambertian(Vec3(1.0, 0.0, 0.0), 1.0) :> IMaterial)
            (Sphere(Vec3(1.0, 0.0, -1.0), 0.5) :> GeometricObject, Lambertian(Vec3(0.0, 0.0, 1.0), 1.0) :> IMaterial)
        
            //(InfPlane(Vec3(0.0, -0.5, -1.0), Vec3(0.0, 1.0, 0.0)) :> GeometricObject, Lambertian(Vec3(0.5, 0.5, 0.5), 1.0) :> IMaterial)
            //(Plane(Vec3(-1.0, -0.5, 1.0), Vec3(2.0, 0.0, 0.0), Vec3(0.0, 0.0, -2.0)) :> GeometricObject, Lambertian(Vec3(0.8, 0.8, 0.0), 1.0) :> IMaterial)
            //(Plane(Vec3(-1.5, -1.0, 1.0), Vec3(0.0, 0.0, -2.0), Vec3(0.0, 2.0, 0.0)) :> GeometricObject, Lambertian(Vec3(0.0, 0.8, 0.8), 1.0) :> IMaterial)
            //(Plane(Vec3(1.5, -1.0, 1.0), Vec3(0.0, 0.0, -2.0), Vec3(0.0, 2.0, 0.0)) :> GeometricObject, Lambertian(Vec3(0.0, 0.8, 0.8), 1.0) :> IMaterial)
        
            (Plane(Vec3(-2.0, -0.6, 0.0), Vec3(4.0, 0.0, 0.0), Vec3(0.0, 0.0, -3.5)) :> GeometricObject, Lambertian(Vec3(0.5, 0.5, 0.5), 0.6) :> IMaterial)
            (Triangle(Vec3(0.0, -0.5, -1.0), Vec3(1.5, -0.3, -2.5), Vec3(0.5, 0.8, -1.5)) :> GeometricObject, Lambertian(Vec3(0.8, 0.8, 0.0), 1.0) :> IMaterial)
        ]

        let lightList : list<ILight> = [
            (AmbientLight(Vec3(0.03, 0.03, 0.03)) :> ILight)
            (ParallelLight(Vec3(-1.0, -1.0, 0.0), Vec3(0.5, 0.5, 0.5)) :> ILight)
        ]

        let vp = ViewPlane(width, height, 12.0 / float height, 1, 1.0)
        let camera = PinholeCamera(Vec3(0.0, 3.0, 3.0), Vec3(0.0, 0.0, -1.0), Vec3(0.0, 1.0, 0.0), 30.0)
        let world = World(vp, Vec3.Zero, hitableList, lightList, camera :> ICamera)
        
        // table with three rows
        let layout = new StackLayout()
        let imageView = new ImageView()
        imageView.Image <- new Bitmap(Size(width, height), PixelFormat.Format24bppRgb);
        let progressBar = new ProgressBar()
        progressBar.MaxValue <- 1000
        progressBar.Width <- width
        layout.Items.Add(new StackLayoutItem(imageView))
        layout.Items.Add(new StackLayoutItem(progressBar))

        let timer = new UITimer()
        timer.Interval <- 1.0
        timer.Elapsed.Add(fun _ -> imageView.Image <- world.ViewPlane.RenderedImage)
        timer.Start()

        // MENU

        base.Menu <- new MenuBar()
        let openCommand = new Command(MenuText = "Open")
        openCommand.Shortcut <- Application.Instance.CommonModifier ||| Keys.O
        openCommand.Executed.Add(fun e -> MessageBox.Show(this, "file open") |> ignore)

        let quitCommand = new Command(MenuText = "Quit")
        quitCommand.Shortcut <- Application.Instance.CommonModifier ||| Keys.Q
        quitCommand.Executed.Add(fun e -> Application.Instance.Quit())

        let fileItem = new ButtonMenuItem(Text = "&File")
        fileItem.Items.Add(openCommand) |> ignore
        base.Menu.Items.Add(fileItem)
        base.Menu.QuitItem <- quitCommand.CreateMenuItem()

        // World menu

        let worldSettingCommand = new Command(MenuText = "World")
        worldSettingCommand.Executed.Add(fun e -> MessageBox.Show(this, "setting") |> ignore)
        let viewplaneSettingCommand = new Command(MenuText = "Viewplane")
        viewplaneSettingCommand.Executed.Add(fun e -> MessageBox.Show(this, "setting") |> ignore)
        let cameraSettingCommand = new Command(MenuText = "Camera")
        cameraSettingCommand.Executed.Add(fun e -> MessageBox.Show(this, "setting") |> ignore)
        let objectsSettingCommand = new Command(MenuText = "Objects")
        objectsSettingCommand.Executed.Add(fun e -> MessageBox.Show(this, "setting") |> ignore)
        let lightsSettingCommand = new Command(MenuText = "Lights")
        lightsSettingCommand.Executed.Add(fun e -> MessageBox.Show(this, "setting") |> ignore)

        let settingItem = new ButtonMenuItem(Text = "&Setting")
        settingItem.Items.Add(worldSettingCommand) |> ignore
        settingItem.Items.Add(viewplaneSettingCommand) |> ignore
        settingItem.Items.Add(cameraSettingCommand) |> ignore
        settingItem.Items.Add(objectsSettingCommand) |> ignore
        settingItem.Items.Add(lightsSettingCommand) |> ignore
        base.Menu.Items.Add(settingItem)
        
        // Render menu

        let renderStartCommand = new Command(MenuText = "Start")
        let startToolStripMenuItemClicked = fun _ ->
            //cts <- new CancellationTokenSource()
            this.ClientSize <- new Size(world.ViewPlane.Width, world.ViewPlane.Height + progressBar.Height)
            progressBar.Visible <- true
            
            let size = new Drawing.Size(world.ViewPlane.Width, world.ViewPlane.Height)
            let mutable renderedRay = 0
            let jobCompleted = fun (x, y, col) -> 
                world.ViewPlane.SetPixel(x, y, col)
                renderedRay <- renderedRay + 1
                if renderedRay % (1000 * world.ViewPlane.NumSamples) = 0 then
                    let renderedPixel = int (floor (float renderedRay / float world.ViewPlane.NumSamples))
                    printfn "RenderedPixels:%A  Percent:%.2f%%"
                            renderedPixel
                            (float renderedPixel * 100.0 / float (size.Width * size.Height))
                progressBar.Value <- int (float renderedRay / float world.ViewPlane.NumSamples * float progressBar.MaxValue / float (size.Width * size.Height))
            do RenderScene world jobCompleted
        renderStartCommand.Shortcut <- Application.Instance.CommonModifier ||| Keys.R
        renderStartCommand.Executed.Add(fun _ -> startToolStripMenuItemClicked() |> ignore)

        let renderStopCommand = new Command(MenuText = "Stop")
        renderStopCommand.Executed.Add(fun e -> MessageBox.Show(this, "Stop render") |> ignore)

        let renderItem = new ButtonMenuItem(Text = "&Render")
        renderItem.Items.Add(renderStartCommand) |> ignore
        renderItem.Items.Add(renderStopCommand) |> ignore
        base.Menu.Items.Add(renderItem)
        
        base.ClientSize <- new Size(width, imageView.Height + progressBar.Height)
        base.Content <- layout;
