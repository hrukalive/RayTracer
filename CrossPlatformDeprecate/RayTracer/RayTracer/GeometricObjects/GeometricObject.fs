namespace RayTracer.GeometricObjects

open RayTracer.BaseTypes

[<Struct>]
type HitRecord = {
    Normal : Vec3 
    Position : Vec3
    RayT : float
}

type GeometricObject = 
    abstract member Hit : Ray -> HitRecord option
