namespace RayTracerBase.Material

open RayTracerBase.BaseTypes
open RayTracerBase.GeometricObjects
open RayTracerBase.Material.MaterialFuncs

type Metal(albedo : Vec3) = 
    interface IMaterial with
        member this.Scatter(ray : Ray, record : HitRecord) : (Ray option * Vec3) =
            let reflected = Reflect ray.DirectionNorm record.Normal
            let scattered = Ray(record.Position, reflected)

            if (Vec3Ops.Dot scattered.Direction record.Normal) > 0.0 then
                (Some(scattered), albedo)
            else
                (None, albedo)

        member this.Emitted with get() = Vec3.Zero
