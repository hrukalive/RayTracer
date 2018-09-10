namespace RayTracer.GeometricObjects

open RayTracer.BaseTypes

type Plane(a : Vec3, n : Vec3) =
    interface GeometricObject with
        member this.Hit(ray : Ray) : HitRecord option =
            let temp = (Vec3Ops.Dot (a - ray.Origin) n) / (Vec3Ops.Dot ray.Direction n)

            match temp > kEpsilon with
            | true -> 
                Some({ Normal = n
                       Position = ray.GetPoint(temp)
                       RayT = temp })
            | false -> None
