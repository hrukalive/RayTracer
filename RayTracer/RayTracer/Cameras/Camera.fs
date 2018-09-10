namespace RayTracer

module Camera = 
    open System
    open BaseTypes
    open Material.MaterialFuncs
    
    type ICamera = 
        abstract member CreateRay : viewplane : ViewPlane -> c : int * r : int * s : int -> Ray
        
    type OrthographicCamera(eyePoint : Vec3, lookAt : Vec3, up : Vec3) = 
        class
            let w = (eyePoint - lookAt) |> Vec3Ops.Normalize
            let u = (Vec3Ops.Cross up w) |> Vec3Ops.Normalize
            let v = (Vec3Ops.Cross w u) |> Vec3Ops.Normalize

            interface ICamera with
                override this.CreateRay (viewplane : ViewPlane) (c : int, r : int, s : int) : Ray = 
                    let spx, spy = viewplane.GetSamplePoint(c, r, s)
                    let ray = Ray(spx * u + spy * v, -w)
                    ray
        end
    type PinholeCamera(eyePoint : Vec3, lookAt : Vec3, up : Vec3, dist : float) = 
        class
            let w = (eyePoint - lookAt) |> Vec3Ops.Normalize
            let u = (Vec3Ops.Cross up w) |> Vec3Ops.Normalize
            let v = (Vec3Ops.Cross w u) |> Vec3Ops.Normalize
            
            interface ICamera with
                override this.CreateRay (viewplane : ViewPlane) (c : int, r : int, s : int) : Ray = 
                    let spx, spy = viewplane.GetSamplePoint(c, r, s)
                    let ray = Ray(eyePoint, Vec3Ops.Normalize (spx * u + spy * v - dist * w))
                    ray
        end