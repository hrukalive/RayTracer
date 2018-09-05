namespace RayTracerBase.Material

open System
open RayTracerBase.BaseTypes
open RayTracerBase.GeometricObjects

type IMaterial = 
    abstract member Scatter : Ray * HitRecord -> (Ray option * Vec3)
    abstract Emitted : Vec3 with get

module MaterialFuncs =
    let GetDiffuseDirection (rand:Random) =
        let p = 2.0 * Vec3(rand.NextDouble(),rand.NextDouble(),rand.NextDouble()) - Vec3.One
        (Vec3Ops.Normalize p) * rand.NextDouble()

    let Reflect (vin:Vec3) (normal:Vec3) = 
        vin - 2.0 * (Vec3Ops.Dot vin normal) * normal

    let Refract (vin:Vec3) (normal:Vec3) (niNo:float) : Vec3 option =
        let uvin = Vec3Ops.Normalize vin
        let dt = Vec3Ops.Dot uvin normal
        let disc = 1.0 - niNo * niNo * (1.0 - dt * dt)
        match disc with
        | disc when disc > 0.0 ->
            Some(niNo * (uvin - normal * dt) - normal * Math.Sqrt(disc))
        | _ -> None

    let Schlick (cos : float) (refIdx : float) =
        let r = (1.0 - refIdx) / (1.0 + refIdx)
        r * r + (1.0 - r * r) * (1.0 - cos) ** 5.0