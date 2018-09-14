namespace RayTracer.Light

open RayTracer.BaseTypes

type ILight = 
    abstract member Color : Vec3 with get

type AmbientLight(color : Vec3) = 
    interface ILight with
        override this.Color with get() = color
        
type ParallelLight(direction: Vec3, color : Vec3) = 
    member this.Direction with get() = direction
    interface ILight with
        override this.Color with get() = color

type PointLight(location: Vec3, color : Vec3) = 
    member this.Location with get() = location
    interface ILight with
        override this.Color with get() = color