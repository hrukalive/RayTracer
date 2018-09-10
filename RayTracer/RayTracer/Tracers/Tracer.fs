namespace RayTracer

module Tracer = 
    open System
    open BaseTypes
    open RayTracer.GeometricObjects
    open RayTracer.Material

    type TraceRecord = { 
        HitRecord : HitRecord
        Material : IMaterial
    }

    let HitObjects (ray : Ray) (tmax: float) (objs : list<GeometricObject * IMaterial>) : TraceRecord option =
        let mutable closest = tmax
        let mutable record : TraceRecord option = None
        for (geometricObject, material) in objs do
            match geometricObject.Hit(ray) with
            | Some(hitRecord) -> 
                if hitRecord.RayT < closest then
                    closest <- hitRecord.RayT
                    record <- Some({ HitRecord = hitRecord; Material = material })
            | None -> ()
        record

    let rec TraceRay (ray : Ray) (objs : list<GeometricObject * IMaterial>) depth maxDepth : Vec3 =
        match HitObjects ray Double.MaxValue objs with
        | Some(record) -> 
            let (ray, atten) = record.Material.Scatter(ray, record.HitRecord)
            if ray.IsSome && depth < maxDepth then
                let col = TraceRay ray.Value objs (depth + 1) maxDepth
                Vec3(col.X * atten.X, col.Y * atten.Y, col.Z * atten.Z) + record.Material.Emitted
            else
                record.Material.Emitted
        | None -> Vec3.Zero
