namespace RayTracerBase.GeometricObjects

open RayTracerBase.BaseTypes

type Sphere(center : Vec3, radius : float) =
    interface GeometricObject with
        member this.Hit(ray : Ray, tmin : float, tmax : float) : HitRecord option=
            let temp = ray.Orginal - center
            let a = (Vec3Ops.Dot ray.Direction ray.Direction)
            let b = 2.0 * (Vec3Ops.Dot temp ray.Direction)
            let c = (Vec3Ops.Dot temp temp) - radius * radius
            let disc = b * b - 4.0 * a * c

            match disc < 0.0 with
            | true -> None
            | false ->
                let t1 = (-b - sqrt(float disc)) / a * 0.5;
                let t2 = lazy ((-b + sqrt(disc)) / a * 0.5);
                let t = (t1, t2)

                match t with
                | (t1, _) when t1 > tmin && t1 < tmax ->
                    let position = ray.GetPoint(t1)
                    let normal = Vec3Ops.Normalize (position - center)

                    Some({ Normal = normal
                           Position = position
                           RayT = t1 })
                | (_, t2) when t2.Force() > tmin && t2.Force() < tmax ->
                    let position = ray.GetPoint(t2.Force())
                    let normal = Vec3Ops.Normalize (position - center)

                    Some({ Normal = normal
                           Position = position
                           RayT = t2.Force() })
                | _ -> None
