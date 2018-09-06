namespace RayTracerBase

module World = 
    open System
    open System.Threading
    open FSharp.Collections.ParallelSeq
    open BaseTypes
    open BaseFunctions
    open RayTracerBase.GeometricObjects
    open RayTracerBase.Material
    open RayTracer
    open ViewPlane

    type World = 
        struct
            val ViewPlane : ViewPlane
            val BgColor : Vec3
            val Objects : list<GeometricObject * IMaterial>
            val Camera : Camera
            new (viewplane : ViewPlane, bgColor : Vec3, objects : list<GeometricObject * IMaterial>, camera : Camera) = {
                ViewPlane = viewplane;
                BgColor = bgColor;
                Objects = objects;
                Camera = camera;
            }
        end

    let GenerateRenderTasks (world : World) = 
        let size = new Drawing.Size(world.ViewPlane.Width, world.ViewPlane.Height)
        let spp = world.ViewPlane.NumSamples
        let objs = world.Objects
        let camera = world.Camera
        let (xRecip, yRecip) = (1.0 / float size.Width, 1.0 / float size.Height)
        let colWeigth = 1.0 / float spp
    
        let rng = new Random()
        let Shuffle (org:_[]) = 
            let arr = Array.copy org
            let max = (arr.Length - 1)
            let randomSwap (arr:_[]) i =
                let pos = rng.Next(max)
                let tmp = arr.[pos]
                arr.[pos] <- arr.[i]
                arr.[i] <- tmp
                arr
   
            [|0..max|] |> Array.fold randomSwap arr
    
        printfn "Generating rendering tasks"
        //let tasks = 
        //    [| 1 .. world.ViewPlane.NumSamples |]
        //    |> PSeq.map (fun _ ->
        //        [| 0 .. (size.Height * size.Width - 1) |]
        //        |> PSeq.map (fun pxid ->
        //            async {
        //                let random = System.Random()
        //                let x = pxid % size.Width
        //                let y = pxid / size.Width
        //                let xNorm = float x / float size.Width
        //                let yNorm = 1.0 - float y / float size.Height
                    
        //                let xTrace = xNorm + random.NextDouble() * xRecip
        //                let yTrace = yNorm + random.NextDouble() * yRecip
        //                let ray = camera.CreateRay(xTrace, yTrace)
        //                let col = TraceRay ray objs 0 5
                
        //                return (x, y, col |> Gamma) 
        //            })
        //        |> PSeq.toArray
        //        |> Shuffle
        //        )
        //    |> PSeq.concat
        //    |> PSeq.toArray
        let tasks = 
            [| 0 .. (size.Height * size.Width - 1) |]
            |> PSeq.map (fun pxid ->
                async {
                    let random = System.Random()
                    let x = pxid % size.Width
                    let y = pxid / size.Width
                    let xNorm = float x / float size.Width
                    let yNorm = 1.0 - float y / float size.Height

                    let mutable col = Vec3.Zero

                    for _ in 1 .. spp do
                        let xTrace = xNorm + random.NextDouble() * xRecip
                        let yTrace = yNorm + random.NextDouble() * yRecip
                        let ray = camera.CreateRay(xTrace,yTrace)
                        col <- col + colWeigth * (TraceRay ray objs 0 5)
                
                    return (x, y, col |> Gamma) 
                })
            |> PSeq.toArray
            |> Shuffle
        printfn "Rendering tasks generated"
        tasks

    let RenderScene (world : World) cts jobCompleted completeCallback cancelCallback = 
        let exceptionContinuation (ex : exn) = 
            printfn "Error: %s" ex.Message
            printfn "%s" ex.StackTrace
            // MessageBox.Show("Error: " + ex.Message + "\n" + ex.StackTrace, "Ray Tracer", MessageBoxButtons.OK, MessageBoxIcon.Error) |> ignore
    
        ResetRenderedImage world.ViewPlane
        printfn "Reset viewplane image"
    
        let worker = new AsyncWorker<_>(GenerateRenderTasks world, completeCallback, exceptionContinuation, cancelCallback, cts)
        worker.JobCompleted.Add(jobCompleted)
        async { worker.Start() } |> Async.Start
