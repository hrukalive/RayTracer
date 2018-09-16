namespace RayTracer.GeometricObjects

open RayTracer.BaseTypes

type InfPlane(a : Vec3, n : Vec3) =
    interface GeometricObject with
        member this.Hit(ray : Ray) : HitRecord option =
            let t = (Vec3Ops.Dot (a - ray.Origin) n) / (Vec3Ops.Dot ray.Direction n)
            match t > kEpsilon with
            | true -> 
                Some({ Normal = Vec3Ops.Normalize n
                       Position = ray.GetPoint(t)
                       RayT = t })
            | false -> None

type Plane(a : Vec3, u : Vec3, v: Vec3) =
    let n : Vec3 = Vec3Ops.Cross u v
    interface GeometricObject with
        member this.Hit(ray : Ray) : HitRecord option =
            let t = (Vec3Ops.Dot (a - ray.Origin) n) / (Vec3Ops.Dot ray.Direction n)
            let p = ray.GetPoint(t)
            let tu = (Vec3Ops.Dot (p - a) (Vec3Ops.Normalize u)) / (Vec3Ops.Norm u)
            let tv = (Vec3Ops.Dot (p - a) (Vec3Ops.Normalize v)) / (Vec3Ops.Norm v)

            match t > kEpsilon && tu >= 0.0 && tu <= 1.0 && tv >= 0.0 && tv <= 1.0 with
            | true -> 
                Some({ Normal = Vec3Ops.Normalize n
                       Position = p
                       RayT = t })
            | false -> None
