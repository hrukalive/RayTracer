namespace RayTracer

module World = 
    open System
    open System.Threading
    open FSharp.Collections.ParallelSeq
    open BaseTypes
    open BaseFunctions
    open RayTracer.GeometricObjects
    open RayTracer.Material
    open Tracer
    open Camera
    open System.Threading.Tasks
    open System.Drawing
    open Sampler
    open RayTracer.Light
    open System.Windows.Forms

    type World = 
        struct
            val ViewPlane : ViewPlane
            val BgColor : Vec3
            val Objects : list<GeometricObject * IMaterial>
            val Lights : list<ILight>
            val Camera : ICamera
            new (viewplane : ViewPlane, bgColor : Vec3, objects : list<GeometricObject * IMaterial>, lights : list<ILight>, camera : ICamera) = {
                ViewPlane = viewplane;
                BgColor = bgColor;
                Objects = objects;
                Lights = lights;
                Camera = camera;
            }
        end

        
    let (width, height) = (128*5, 72*5)

    let hitableList : list<GeometricObject* IMaterial> = [
        (Sphere(Vec3(0.0, 0.0, -2.0), 0.5) :> GeometricObject, Lambertian(Vec3(0.0, 1.0, 0.0), 1.0) :> IMaterial)
        (Sphere(Vec3(-1.0, 0.0, -1.0), 0.5) :> GeometricObject, Lambertian(Vec3(1.0, 0.0, 0.0), 1.0) :> IMaterial)
        (Sphere(Vec3(1.0, 0.0, -1.0), 0.5) :> GeometricObject, Lambertian(Vec3(0.0, 0.0, 1.0), 1.0) :> IMaterial)
        (Plane(Vec3(-2.0, -0.6, 0.0), Vec3(4.0, 0.0, 0.0), Vec3(0.0, 0.0, -3.0)) :> GeometricObject, Lambertian(Vec3(0.5, 0.5, 0.5), 0.6) :> IMaterial)
        (Triangle(Vec3(0.0, -0.5, -1.0), Vec3(1.5, -0.3, -2.5), Vec3(0.5, 0.8, -1.5)) :> GeometricObject, Lambertian(Vec3(0.8, 0.8, 0.0), 1.0) :> IMaterial)
        (Plane(Vec3(-0.4, -0.4, -0.5), Vec3(0.0, 0.0, -1.0), Vec3(0.0, 0.7, 0.0)) :> GeometricObject, Lambertian(Vec3(0.0, 0.8, 0.8), 1.0) :> IMaterial)
    ]

    let lightList : list<ILight> = [
        (AmbientLight(Vec3(0.05, 0.05, 0.05)) :> ILight)
        (ParallelLight(Vec3(-1.0, -1.0, 0.0), Vec3(0.5, 0.5, 0.5)) :> ILight)
    ]

    //let vp = ViewPlane(width, height, 3.0 / float height, 64, 1.0) // Ortho config
    let vp = ViewPlane(width, height, 1.0 / float height, 64, 1.0) // Persp config
    let r = 3.0
    let theta = 0.0 / 180.0 * 3.1416
    let phi = 45.0 / 180.0 * 3.1416
    let roll = 0.0 / 180.0 * 3.1416
    let lookat = Vec3(0.0, -0.6, -1.5)
    let eyepoint = Vec3(r * Math.Sin(theta) * Math.Sin(phi), r * Math.Cos(phi), r * Math.Cos(theta) * Math.Sin(phi)) + lookat
    let camera = PinholeCamera(eyepoint, lookat, Vec3(Math.Sin(roll), Math.Cos(roll), 0.0), 1.0)
    let camera2 = OrthographicCamera(eyepoint, lookat, Vec3(Math.Sin(roll), Math.Cos(roll), 0.0))
    let world = World(vp, Vec3.Zero, hitableList, lightList, camera :> ICamera)

    let GenerateRenderTasks (preview : bool) (world : World) (syncContext : SynchronizationContext) (jobCompleteCallback : (int * int * Vec3) -> unit) = 
        let size = new Drawing.Size(world.ViewPlane.Width, world.ViewPlane.Height)
        let spp = world.ViewPlane.NumSamples
        let objs = world.Objects
        let lights = world.Lights
        let camera = world.Camera
    
        printfn "Generating rendering tasks"
        
        let blockSize = 128
        let xBlockCount = int (Math.Ceiling (float size.Width / float blockSize))
        let yBlockCount = int (Math.Ceiling (float size.Height / float blockSize))
        let Sampler = (if preview then OneSample() :> ISampler else MultiJittered(world.ViewPlane.NumSamples) :> ISampler)
        let RayCreator = camera.CreateRay world.ViewPlane Sampler
        
        if preview then
            seq { 
                yield ([| 0 .. (xBlockCount * yBlockCount - 1) |]
                    |> PSeq.map (fun blockId -> 
                        let xBlock = blockId % xBlockCount
                        let yBlock = blockId / xBlockCount
                        let xStart, xEnd = xBlock * blockSize, Math.Min((xBlock + 1) * blockSize, size.Width)
                        let yStart, yEnd = yBlock * blockSize, Math.Min((yBlock + 1) * blockSize, size.Height)
                        let xSize, ySize = xEnd - xStart, yEnd - yStart

                        [| 0 .. (xSize * ySize - 1) |]
                        |> PSeq.map (fun pxid ->
                            async {
                                let c = pxid % xSize + xStart
                                let r = pxid / xSize + yStart

                                let ray = (RayCreator (c, r)).[0]
                                let color = TraceRay ray objs lights 0 5

                                do! Async.SwitchToContext syncContext
                                lock world.ViewPlane.RenderLock (fun () -> 
                                    jobCompleteCallback(c, r, color |> Gamma)
                                )
                            }
                        )
                        |> PSeq.toArray
                        |> Shuffle
                        |> Async.Parallel
                    )
                    |> PSeq.toArray
                    |> Async.Parallel
                    |> Async.Ignore)
                }
        else
            Seq.init (xBlockCount * yBlockCount) (fun blockId ->
                let xBlock = blockId % xBlockCount
                let yBlock = blockId / xBlockCount
                let xStart, xEnd = xBlock * blockSize, Math.Min((xBlock + 1) * blockSize, size.Width)
                let yStart, yEnd = yBlock * blockSize, Math.Min((yBlock + 1) * blockSize, size.Height)
                let xSize, ySize = xEnd - xStart, yEnd - yStart
                [| 0 .. (xSize * ySize - 1) |]
                |> PSeq.map (fun pxid ->
                    let c = pxid % xSize + xStart
                    let r = pxid / xSize + yStart
                    RayCreator (c, r)
                    |> PSeq.map (fun ray ->
                        async {
                            let color = TraceRay ray objs lights 0 5
                            
                            do! Async.SwitchToContext syncContext
                            lock world.ViewPlane.RenderLock (fun () -> 
                                jobCompleteCallback(c, r, color |> Gamma)
                            )
                        }
                    )
                    |> PSeq.toArray
                    |> Async.Parallel
                )
                |> PSeq.toArray
                |> Shuffle
                |> Async.Parallel
                |> Async.Ignore
            )

    let RenderScene (world : World) jobCompleted (cts : CancellationTokenSource) = // completeCallback cancelCallback = 
        let exceptionContinuation (ex : exn) = 
            printfn "Error: %s" ex.Message
            printfn "%s" ex.StackTrace
            // MessageBox.Show("Error: " + ex.Message + "\n" + ex.StackTrace, "Ray Tracer", MessageBoxButtons.OK, MessageBoxIcon.Error) |> ignore
        let syncContext = match SynchronizationContext.Current with
                          | null -> new SynchronizationContext()
                          | ctxt -> ctxt
        world.ViewPlane.ResetRenderedImage()
        printfn "Reset viewplane image"
        
        Async.Start(async {
            let poolSyncContext = match SynchronizationContext.Current with
                                  | null -> new SynchronizationContext()
                                  | ctxt -> ctxt
            let vp2 = ViewPlane(width, height, 1.0 / float height, 64, 1.0)
            let world2 = World(vp2, Vec3.Zero, hitableList, lightList, camera :> ICamera)
            let taskSeq = GenerateRenderTasks true world2 poolSyncContext jobCompleted
            let worker = new AsyncWorker(taskSeq, cts)
            //Seq.iter (fun task -> Async.RunSynchronously(task |> Async.Ignore)) taskSeq
            worker.Start()
        })
        
        //task.ContinueWith(completeCallback, TaskContinuationOptions.OnlyOnRanToCompletion)
    
        //let worker = new AsyncWorker<_>(GenerateRenderTasks world, completeCallback, exceptionContinuation, cancelCallback, cts)
        //worker.JobCompleted.Add(jobCompleted)
        //async { worker.Start() } |> Async.Start
