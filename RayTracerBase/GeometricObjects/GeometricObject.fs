namespace RayTracerBase.GeometricObjects

open RayTracerBase.BaseTypes

[<Struct>]
type HitRecord = {
    Normal : Vec3 
    Position : Vec3
    RayT : float
}

type GeometricObject = 
    abstract member Hit : Ray * float * float -> HitRecord option
