namespace RayTracer

module Camera = 
    open System
    open BaseTypes
    open Sampler
    
    type ICamera = 
        abstract member CreateRay : viewplane : ViewPlane -> sampler : ISampler -> c : int * r : int -> Ray[]

    type OrthographicCamera(eyePoint : Vec3, lookAt : Vec3, up : Vec3) = 
        class
            let w = (eyePoint - lookAt) |> Vec3Ops.Normalize
            let u = (Vec3Ops.Cross up w) |> Vec3Ops.Normalize
            let v = (Vec3Ops.Cross w u) |> Vec3Ops.Normalize

            interface ICamera with
                override this.CreateRay viewplane sampler (c, r) = 
                    let spx, spy = (viewplane.GetPixelCenter (c, r))
                    let samples = sampler.SquareSample()
                    Array.map (fun (dpx, dpy) -> Ray((spx + dpx * viewplane.PixelSize) * u + (spy + dpy * viewplane.PixelSize) * v + eyePoint, -w)) samples
        end
    type PinholeCamera(eyePoint : Vec3, lookAt : Vec3, up : Vec3, dist : float) = 
        class
            let w = (eyePoint - lookAt) |> Vec3Ops.Normalize
            let u = (Vec3Ops.Cross up w) |> Vec3Ops.Normalize
            let v = (Vec3Ops.Cross w u) |> Vec3Ops.Normalize
            
            interface ICamera with
                override this.CreateRay viewplane sampler (c, r) = 
                    let spx, spy = (viewplane.GetPixelCenter (c, r))
                    let samples = sampler.SquareSample()
                    Array.map (fun (dpx, dpy) -> Ray(eyePoint, Vec3Ops.Normalize ((spx + dpx * viewplane.PixelSize) * u + (spy + dpy * viewplane.PixelSize) * v - dist * w))) samples
        end