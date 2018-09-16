namespace RayTracer

open System
open System.Threading
open System.Windows.Forms
open System.Drawing
open World
open BaseTypes
open BaseFunctions

type RayTracerMainGUI (world : World) as self = 
    inherit Form(Width = world.ViewPlane.Width, Height = world.ViewPlane.Height + 24)

    let progressBar = new ProgressBar()
    let menuStrip = new MenuStrip()
    let pictureBox = new PictureBox()
    let imageUpdateTimer = new Timers.Timer(1000.0)

    do self.Initialize()

    member this.Initialize() = 
        let worldToolStripMenuItem = new ToolStripMenuItem();
        let viewplaneToolStripMenuItem = new ToolStripMenuItem();
        let cameraToolStripMenuItem = new ToolStripMenuItem();
        let objectsToolStripMenuItem = new ToolStripMenuItem();
        let lightsToolStripMenuItem = new ToolStripMenuItem();
        let renderToolStripMenuItem = new ToolStripMenuItem();
        let startToolStripMenuItem = new ToolStripMenuItem();
        let stopToolStripMenuItem = new ToolStripMenuItem();
        menuStrip.SuspendLayout()
        (pictureBox :> System.ComponentModel.ISupportInitialize).BeginInit()
        this.SuspendLayout()

        progressBar.Location <- new System.Drawing.Point(0, this.Height);
        progressBar.Name <- "renderProgressBar";
        progressBar.Size <- new System.Drawing.Size(this.Width, 23);
        progressBar.Maximum <- 1000
        progressBar.TabIndex <- 0;
        // 
        // menuStrip1
        // 
        menuStrip.Items.AddRange([| worldToolStripMenuItem :> ToolStripItem; 
                                    renderToolStripMenuItem :> ToolStripItem |]);
        menuStrip.Location <- new System.Drawing.Point(0, 0);
        menuStrip.Name <- "menuStrip1";
        menuStrip.Size <- new System.Drawing.Size(this.Width, 24);
        menuStrip.TabIndex <- 1;
        menuStrip.Text <- "menuStrip1";
        // 
        // pictureBox1
        // 
        pictureBox.Location <- new System.Drawing.Point(0, menuStrip.Height);
        pictureBox.Name <- "renderedPicture";
        pictureBox.Size <- new System.Drawing.Size(world.ViewPlane.Width, world.ViewPlane.Height);
        pictureBox.TabIndex <- 2;
        pictureBox.TabStop <- false;
        // 
        // worldToolStripMenuItem
        // 
        worldToolStripMenuItem.DropDownItems.AddRange([| viewplaneToolStripMenuItem :> ToolStripItem;
                                                         cameraToolStripMenuItem :> ToolStripItem;
                                                         objectsToolStripMenuItem :> ToolStripItem;
                                                         lightsToolStripMenuItem :> ToolStripItem |]);
        worldToolStripMenuItem.Name <- "worldToolStripMenuItem";
        worldToolStripMenuItem.Size <- new System.Drawing.Size(51, 20);
        worldToolStripMenuItem.Text <- "World";
        // 
        // viewplaneToolStripMenuItem
        // 
        viewplaneToolStripMenuItem.Name <- "viewplaneToolStripMenuItem";
        viewplaneToolStripMenuItem.Size <- new System.Drawing.Size(180, 22);
        viewplaneToolStripMenuItem.Text <- "Viewplane";
        // 
        // cameraToolStripMenuItem
        // 
        cameraToolStripMenuItem.Name <- "cameraToolStripMenuItem";
        cameraToolStripMenuItem.Size <- new System.Drawing.Size(180, 22);
        cameraToolStripMenuItem.Text <- "Camera";
        // 
        // objectsToolStripMenuItem
        // 
        objectsToolStripMenuItem.Name <- "objectsToolStripMenuItem";
        objectsToolStripMenuItem.Size <- new System.Drawing.Size(180, 22);
        objectsToolStripMenuItem.Text <- "Objects";
        // 
        // lightsToolStripMenuItem
        // 
        lightsToolStripMenuItem.Name <- "lightsToolStripMenuItem";
        lightsToolStripMenuItem.Size <- new System.Drawing.Size(180, 22);
        lightsToolStripMenuItem.Text <- "Lights";
        // 
        // renderToolStripMenuItem
        // 
        renderToolStripMenuItem.DropDownItems.AddRange([| startToolStripMenuItem :> ToolStripItem;
                                                          stopToolStripMenuItem :> ToolStripItem |]);
        renderToolStripMenuItem.Name <- "renderToolStripMenuItem";
        renderToolStripMenuItem.Size <- new System.Drawing.Size(56, 20);
        renderToolStripMenuItem.Text <- "Render";
        // 
        // startToolStripMenuItem
        // 
        let mutable cts = new CancellationTokenSource()
        let finishContinuation = fun _ -> 
            printfn "Rendering completed."
            MessageBox.Show("Rendering complete.", "Ray Tracer", MessageBoxButtons.OK, MessageBoxIcon.Information) |> ignore
            progressBar.Visible <- false
            this.ClientSize <- new Size(world.ViewPlane.Width, world.ViewPlane.Height + menuStrip.Height)
        let cancelContinuation = fun _ -> 
            printfn "Rendering cancelled."
            // MessageBox.Show("Rendering cancelled.", "Ray Tracer", MessageBoxButtons.OK, MessageBoxIcon.Warning) |> ignore
            progressBar.Visible <- false
            this.ClientSize <- new Size(world.ViewPlane.Width, world.ViewPlane.Height + menuStrip.Height)
        let startToolStripMenuItemClicked = fun _ ->
            cts <- new CancellationTokenSource()
            this.ClientSize <- new Size(world.ViewPlane.Width, world.ViewPlane.Height + menuStrip.Height + progressBar.Height)
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
                progressBar.Style <- ProgressBarStyle.Continuous
                progressBar.Value <- int (float renderedRay / float world.ViewPlane.NumSamples * float progressBar.Maximum / float (size.Width * size.Height))
            progressBar.Style <- ProgressBarStyle.Marquee
            do RenderScene world jobCompleted cts
        startToolStripMenuItem.Name <- "startToolStripMenuItem";
        startToolStripMenuItem.Size <- new System.Drawing.Size(180, 22);
        startToolStripMenuItem.Text <- "Start";
        startToolStripMenuItem.Click.Add(startToolStripMenuItemClicked)
        // 
        // stopToolStripMenuItem
        // 
        stopToolStripMenuItem.Name <- "stopToolStripMenuItem";
        stopToolStripMenuItem.Size <- new System.Drawing.Size(180, 22);
        stopToolStripMenuItem.Text <- "Cancel";
        stopToolStripMenuItem.Click.Add(fun _ -> cts.Cancel())

        this.AutoSize <- false
        this.AutoScaleMode <- AutoScaleMode.Font
        this.Text <- "RayTracer"
        this.ClientSize <- new Size(world.ViewPlane.Width, world.ViewPlane.Height + menuStrip.Height)
        this.MaximizeBox <- false
        this.FormBorderStyle <- FormBorderStyle.FixedSingle
        
        this.Controls.Add(pictureBox);
        this.Controls.Add(progressBar);
        this.Controls.Add(menuStrip);
        
        menuStrip.ResumeLayout(false);
        menuStrip.PerformLayout();
        (pictureBox :> System.ComponentModel.ISupportInitialize).EndInit();
        this.ResumeLayout(false);
        this.PerformLayout();

        pictureBox.DoubleClick.Add(fun _ ->
            pictureBox.Image.Save("Result.bmp")
            System.Diagnostics.Process.Start("Result.bmp") |> ignore)
            
        let ctx = SynchronizationContext.Current
        imageUpdateTimer.Elapsed.Add(fun _ -> 
            async {
                do! Async.SwitchToContext ctx
                pictureBox.Image <- world.ViewPlane.RenderedImage
                pictureBox.Update()
            } |> Async.StartImmediate
        )
        imageUpdateTimer.Start()

    member this.ProgressBar with get() = progressBar
    member this.Menu with get() = menuStrip
    member this.PictureBox with get() = pictureBox


