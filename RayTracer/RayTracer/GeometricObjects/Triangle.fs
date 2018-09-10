namespace RayTracer.GeometricObjects

open RayTracer.BaseTypes

type Triangle(a : Vec3, b : Vec3, c : Vec3) =
    let n = Vec3Ops.Normalize (Vec3Ops.Cross (b - a) (c - a))
    interface GeometricObject with
        member this.Hit(ray : Ray) : HitRecord option =
            let temp = (Vec3Ops.Dot (a - ray.Origin) n) / (Vec3Ops.Dot ray.Direction n)

            match temp > kEpsilon with
            | true -> 
                let p = ray.GetPoint(temp)
                let n = Vec3Ops.Cross (b - a) (c - a)
                let denom = Vec3Ops.Dot n n
                let l1 = (Vec3Ops.Dot n (Vec3Ops.Cross (b - a) (p - a))) / denom
                let l2 = (Vec3Ops.Dot n (Vec3Ops.Cross (c - b) (p - b))) / denom
                let l3 = 1.0 - l1 - l2
                match 0.0 <= l1 && l1 <= 1.0 && 0.0 <= l2 && l2 <= 1.0 && 0.0 <= l3 && l3 <= 1.0 with
                | true ->
                    Some({ Normal = Vec3Ops.Normalize n
                           Position = p
                           RayT = temp })
                | false -> None
            | false -> None
