namespace RayTracer

open System
open System.Threading
open FSharp.Collections.ParallelSeq

module BaseTypes = 
    [<Literal>]
    let kEpsilon = 0.001

    type Vec3 = class
        val X : float
        val Y : float
        val Z : float

        new (c : float) = { X = c; Y = c; Z = c }
        new (x : float, y : float, z : float) = { X = x; Y = y; Z = z}
        new (v : Vec3) = { X = v.X; Y = v.Y; Z = v.Z }
    
        static member (+) (v : Vec3, a : Vec3) = Vec3(v.X + a.X, v.Y + a.Y, v.Z + a.Z)
        static member (~-) (v : Vec3) = Vec3(-1.0 * v.X, -1.0 * v.Y, -1.0 * v.Z)
        static member (-) (v : Vec3, a : Vec3) = Vec3(v.X - a.X, v.Y - a.Y, v.Z - a.Z)
        static member (*) (v : Vec3, a : float) = Vec3(a * v.X, a * v.Y, a * v.Z)
        static member (*) (a : float, v: Vec3) = Vec3(a * v.X, a * v.Y, a * v.Z)
        static member (*) (a: Vec3, v: Vec3) = Vec3(a.X * v.X, a.Y * v.Y, a.Z * v.Z)
        static member (/) (v : Vec3, a) = Vec3(v.X / a, v.Y / a, v.Z / a)
    
        static member UnitX = Vec3(1.0, 0.0, 0.0)
        static member UnitY = Vec3(0.0, 1.0, 0.0)
        static member UnitZ = Vec3(0.0, 0.0, 1.0)
        static member Zero = Vec3(0.0, 0.0, 0.0)
        static member One = Vec3(1.0, 1.0, 1.0)

        override this.ToString() = sprintf "<%.2f, %.2f, %.2f>" this.X this.Y this.Z
    end

    module Vec3Ops = 
        let Norm (v : Vec3) = sqrt(v.X ** 2.0 + v.Y ** 2.0 + v.Z ** 2.0)
        let Normalize(v : Vec3) = 
            let length = Norm v
            Vec3(v.X / length, v.Y / length, v.Z / length)
        let Dot (u : Vec3) (v : Vec3) = u.X * v.X + u.Y * v.Y + u.Z * v.Z
        let Cross (u : Vec3) (v : Vec3) = Vec3(u.Y * v.Z - u.Z * v.Y, 
                                                u.Z * v.X - u.X * v.Z, 
                                                u.X * v.Y - u.Y * v.X)

    type Ray(org : Vec3, dir : Vec3) = 
        member this.GetPoint (t : float) = org + t * dir
        member this.Origin with get() = org
        member this.Direction with get() = dir

    type AsyncWorker (jobs : seq<Async<unit>>, cts : CancellationTokenSource) = //, continuation : unit -> unit, exnCont : exn -> unit, cancelCont : OperationCanceledException -> unit, cts : CancellationTokenSource) =  
        let jobCompleted  = new Event<unit>()
        let syncContext = match SynchronizationContext.Current with
                            | null -> new SynchronizationContext()
                            | ctxt -> ctxt
        member x.Start() =
            let work = 
                async {
                    for job in jobs do
                        if not cts.IsCancellationRequested then
                            do! job
                }
                //jobs
                //|> PSeq.map (fun job ->
                //    async {
                //        let! result = job
                //        syncContext.Post((fun _ -> jobCompleted.Trigger result), state = null)
                //        return result
                //    })
                //|> Async.Parallel
            printfn "Preparing to start raytracing"
            Async.Start(work, cts.Token)
            //Async.StartWithContinuations(work, continuation, exnCont, cancelCont, cts.Token)
            printfn "Workers started"
        member x.JobCompleted  = jobCompleted.Publish
        member x.Cancel() = cts.Cancel()
