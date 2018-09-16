namespace RayTracer.Material

open System
open RayTracer.BaseTypes
open RayTracer.GeometricObjects

type IMaterial = 
    abstract member Scatter : Ray * HitRecord -> (Ray array option * Vec3)
