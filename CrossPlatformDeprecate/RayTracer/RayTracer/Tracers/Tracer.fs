namespace RayTracer

module Tracer = 
    open System
    open BaseTypes
    open RayTracer.GeometricObjects
    open RayTracer.Material
    open RayTracer.Light

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

    let rec TraceRay (ray : Ray) (objs : list<GeometricObject * IMaterial>) (lights : list<ILight>) depth maxDepth : Vec3 =
        match HitObjects ray Double.MaxValue objs with
        | Some(record) -> 
            let (rays, albeto) = record.Material.Scatter(ray, record.HitRecord)
            let mutable illumination = Vec3.Zero
            for light in lights do
                illumination <- illumination + 
                    match light with
                    | :? ParallelLight as pl -> 
                        match HitObjects (Ray (record.HitRecord.Position, -(Vec3Ops.Normalize pl.Direction))) Double.MaxValue objs with
                        | Some(_) -> Vec3.Zero
                        | None -> light.Color * Math.Max(0.0, Vec3Ops.Dot record.HitRecord.Normal -(Vec3Ops.Normalize pl.Direction))
                    | _ -> light.Color
            albeto * illumination
        | None -> Vec3.Zero
