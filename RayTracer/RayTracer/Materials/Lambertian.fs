namespace RayTracer.Material

open RayTracer.BaseTypes
open RayTracer.GeometricObjects
open RayTracer.Material.MaterialFuncs

type Lambertian(albedo:Vec3) = 
    let mutable emitted = Vec3.Zero
    member this.SetEmitted (col : Vec3) = emitted <- col

    interface IMaterial with
        member this.Scatter(ray : Ray, record : HitRecord) : (Ray option * Vec3) =
            let rand = System.Random(int (ray.Direction.Y * 9999999.0))
            let target = record.Position + record.Normal + GetDiffuseDirection rand
            let ray = Ray(record.Position,target - record.Position)
            (Some(ray), albedo)

        member this.Emitted = emitted
