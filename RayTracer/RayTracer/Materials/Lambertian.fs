﻿namespace RayTracer.Material

open RayTracer.BaseTypes
open RayTracer.GeometricObjects
open RayTracer.Material.MaterialFuncs

type Lambertian(albedo : Vec3, diffuseCoeff : float) = 
    interface IMaterial with
        member this.Scatter(ray : Ray, record : HitRecord) : (Ray array option * Vec3) = 
            (None, diffuseCoeff * albedo)
