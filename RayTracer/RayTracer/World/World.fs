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

    type World = 
        struct
            val ViewPlane : ViewPlane
            val BgColor : Vec3
            val Objects : list<GeometricObject * IMaterial>
            val Camera : ICamera
            new (viewplane : ViewPlane, bgColor : Vec3, objects : list<GeometricObject * IMaterial>, camera : ICamera) = {
                ViewPlane = viewplane;
                BgColor = bgColor;
                Objects = objects;
                Camera = camera;
            }
        end

    let GenerateRenderTasks (world : World) (syncContext : SynchronizationContext) (jobCompleteCallback : (int * int * Vec3) -> unit)= 
        let size = new Drawing.Size(world.ViewPlane.Width, world.ViewPlane.Height)
        let spp = world.ViewPlane.NumSamples
        let objs = world.Objects
        let camera = world.Camera
        let (xRecip, yRecip) = (1.0 / float size.Width, 1.0 / float size.Height)
        let colWeigth = 1.0 / float spp
    
        let Shuffle (org:_[]) = 
            let rng = new Random()
            let arr = Array.copy org
            let max = (arr.Length - 1)
            let randomSwap (arr:_[]) i =
                let pos = rng.Next(max)
                let tmp = arr.[pos]
                arr.[pos] <- arr.[i]
                arr.[i] <- tmp
                arr
            [|0..max|] |> Array.fold randomSwap arr

        let mutable renderedPixel = ref 0
        printfn "Generating rendering tasks"
        
        let blockSize = 128
        let xBlockCount = int (Math.Ceiling (float size.Width / float blockSize))
        let yBlockCount = int (Math.Ceiling (float size.Height / float blockSize))
        let RayCreator = camera.CreateRay world.ViewPlane
        let tasks = 
            Seq.init (spp) (fun sid ->
                [| 0 .. (xBlockCount * yBlockCount - 1) |]
                |> PSeq.map (fun blockId -> 
                    let xBlock = blockId % xBlockCount
                    let yBlock = blockId / xBlockCount
                    let xStart, xEnd = xBlock * blockSize, Math.Min((xBlock + 1) * blockSize, size.Width)
                    let yStart, yEnd = yBlock * blockSize, Math.Min((yBlock + 1) * blockSize, size.Height)
                    let xSize, ySize = xEnd - xStart, yEnd - yStart
                    [| 0 .. (xSize * ySize - 1) |]
                    |> PSeq.map (fun pxid ->
                        async {
                            let random = System.Random()
                            let c = pxid % xSize + xStart
                            let r = pxid / xSize + yStart

                            let ray = RayCreator (c, r, sid)
                            let col = TraceRay ray objs 0 5

                            do! Async.SwitchToContext syncContext
                            lock world.ViewPlane.RenderLock (fun () -> 
                                jobCompleteCallback(c, r, col |> Gamma)
                            )
                        }
                    )
                    |> PSeq.toArray
                    |> Shuffle
                    |> Async.Parallel
                )
                |> PSeq.toArray
                |> Async.Parallel
            )
        printfn "Rendering tasks generated"
        tasks

    let RenderScene (world : World) jobCompleted = // completeCallback cancelCallback = 
        let exceptionContinuation (ex : exn) = 
            printfn "Error: %s" ex.Message
            printfn "%s" ex.StackTrace
            // MessageBox.Show("Error: " + ex.Message + "\n" + ex.StackTrace, "Ray Tracer", MessageBoxButtons.OK, MessageBoxIcon.Error) |> ignore
        let syncContext = match SynchronizationContext.Current with
                          | null -> new SynchronizationContext()
                          | ctxt -> ctxt
        world.ViewPlane.ResetRenderedImage()
        printfn "Reset viewplane image"
        
        async {
            let taskSeq = GenerateRenderTasks world syncContext jobCompleted
            Seq.iter (fun task -> task |> Async.Ignore |> Async.RunSynchronously) taskSeq
        } |> Async.Start
        //task.ContinueWith(completeCallback, TaskContinuationOptions.OnlyOnRanToCompletion)
    
        //let worker = new AsyncWorker<_>(GenerateRenderTasks world, completeCallback, exceptionContinuation, cancelCallback, cts)
        //worker.JobCompleted.Add(jobCompleted)
        //async { worker.Start() } |> Async.Start
