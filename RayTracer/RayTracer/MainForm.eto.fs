namespace RayTracer

open System
open Eto.Forms
open Eto.Drawing

type MainFormBase () = 
    inherit Form()
    member this.InitializeComponent() =
        base.Title <- "Ray Tracer"
        base.ClientSize <- new Size(400, 350)
        
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
        renderStartCommand.Shortcut <- Application.Instance.CommonModifier ||| Keys.R
        renderStartCommand.Executed.Add(fun e -> MessageBox.Show(this, "Start render") |> ignore)

        let renderStopCommand = new Command(MenuText = "Stop")
        renderStopCommand.Executed.Add(fun e -> MessageBox.Show(this, "Stop render") |> ignore)

        let renderItem = new ButtonMenuItem(Text = "&Render")
        renderItem.Items.Add(renderStartCommand) |> ignore
        renderItem.Items.Add(renderStopCommand) |> ignore
        base.Menu.Items.Add(renderItem)
        
        // table with three rows
        let layout = new StackLayout()
        let imageView = new ImageView()
        imageView.Image <- new Bitmap(Size(100, 100), PixelFormat.Format24bppRgb);
        let progressBar = new ProgressBar()
        progressBar.MaxValue <- 1000
        layout.Items.Add(new StackLayoutItem(imageView))
        layout.Items.Add(new StackLayoutItem(progressBar))

        base.Content <- layout;
