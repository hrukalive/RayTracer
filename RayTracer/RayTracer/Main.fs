module Main

open System
open System.Windows.Forms
open RayTracer
open RayTracer.BaseTypes
open RayTracer.BaseFunctions
open RayTracer.GeometricObjects
open RayTracer.Material
open RayTracer.Tracer
open RayTracer.World
open RayTracer.Camera
open RayTracer.Light

[<EntryPoint>]
[<STAThread>]
let main argv = 
    let (width, height) = (128 * 5, 72 * 4)

    let hitableList : list<GeometricObject* IMaterial> = [
        (Sphere(Vec3(0.0, 0.0, -2.0), 0.5) :> GeometricObject, Lambertian(Vec3(0.0, 1.0, 0.0), 1.0) :> IMaterial)
        (Sphere(Vec3(-1.0, 0.0, -1.0), 0.5) :> GeometricObject, Lambertian(Vec3(1.0, 0.0, 0.0), 1.0) :> IMaterial)
        (Sphere(Vec3(1.0, 0.0, -1.0), 0.5) :> GeometricObject, Lambertian(Vec3(0.0, 0.0, 1.0), 1.0) :> IMaterial)
        
        //(InfPlane(Vec3(0.0, -0.5, -1.0), Vec3(0.0, 1.0, 0.0)) :> GeometricObject, Lambertian(Vec3(0.5, 0.5, 0.5), 1.0) :> IMaterial)
        //(Plane(Vec3(-1.0, -0.5, 1.0), Vec3(2.0, 0.0, 0.0), Vec3(0.0, 0.0, -2.0)) :> GeometricObject, Lambertian(Vec3(0.8, 0.8, 0.0), 1.0) :> IMaterial)
        //(Plane(Vec3(-1.5, -1.0, 1.0), Vec3(0.0, 0.0, -2.0), Vec3(0.0, 2.0, 0.0)) :> GeometricObject, Lambertian(Vec3(0.0, 0.8, 0.8), 1.0) :> IMaterial)
        //(Plane(Vec3(1.5, -1.0, 1.0), Vec3(0.0, 0.0, -2.0), Vec3(0.0, 2.0, 0.0)) :> GeometricObject, Lambertian(Vec3(0.0, 0.8, 0.8), 1.0) :> IMaterial)
        
        (Plane(Vec3(-2.0, -0.6, 0.0), Vec3(4.0, 0.0, 0.0), Vec3(0.0, 0.0, -3.5)) :> GeometricObject, Lambertian(Vec3(0.5, 0.5, 0.5), 0.6) :> IMaterial)
        (Triangle(Vec3(0.0, -0.5, -1.0), Vec3(1.5, -0.3, -2.5), Vec3(0.5, 0.8, -1.5)) :> GeometricObject, Lambertian(Vec3(0.8, 0.8, 0.0), 1.0) :> IMaterial)
    ]

    let lightList : list<ILight> = [
        (AmbientLight(Vec3(0.03, 0.03, 0.03)) :> ILight)
        (ParallelLight(Vec3(-1.0, -1.0, 0.0), Vec3(0.5, 0.5, 0.5)) :> ILight)
    ]

    let vp = ViewPlane(width, height, 12.0 / float height, 1, 1.0)
    let camera = PinholeCamera(Vec3(0.0, 3.0, 3.0), Vec3(0.0, 0.0, -1.0), Vec3(0.0, 1.0, 0.0), 30.0)
    let world = World(vp, Vec3.Zero, hitableList, lightList, camera :> ICamera)

    Application.EnableVisualStyles()
    Application.SetCompatibleTextRenderingDefault false
    use form = new RayTracerMainGUI(world)
    Application.Run(form)
    0
