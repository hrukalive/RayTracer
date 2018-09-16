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
    let (width, height) = (128 * 5, 72 * 5)

    let hitableList : list<GeometricObject* IMaterial> = [
        (Sphere(Vec3(0.0, 0.0, -2.0), 0.5) :> GeometricObject, Lambertian(Vec3(0.0, 1.0, 0.0), 1.0) :> IMaterial)
        (Sphere(Vec3(-1.0, 0.0, -1.0), 0.5) :> GeometricObject, Lambertian(Vec3(1.0, 0.0, 0.0), 1.0) :> IMaterial)
        (Sphere(Vec3(1.0, 0.0, -1.0), 0.5) :> GeometricObject, Lambertian(Vec3(0.0, 0.0, 1.0), 1.0) :> IMaterial)
        (Plane(Vec3(-2.0, -0.6, 0.0), Vec3(4.0, 0.0, 0.0), Vec3(0.0, 0.0, -3.0)) :> GeometricObject, Lambertian(Vec3(0.5, 0.5, 0.5), 0.6) :> IMaterial)
        (Triangle(Vec3(0.0, -0.5, -1.0), Vec3(1.5, -0.3, -2.5), Vec3(0.5, 0.8, -1.5)) :> GeometricObject, Lambertian(Vec3(0.8, 0.8, 0.0), 1.0) :> IMaterial)
        (Plane(Vec3(-0.4, -0.4, -0.5), Vec3(0.0, 0.0, -1.0), Vec3(0.0, 0.7, 0.0)) :> GeometricObject, Lambertian(Vec3(0.0, 0.8, 0.8), 1.0) :> IMaterial)
    ]

    let lightList : list<ILight> = [
        (AmbientLight(Vec3(0.05, 0.05, 0.05)) :> ILight)
        (ParallelLight(Vec3(-1.0, -1.0, 0.0), Vec3(0.5, 0.5, 0.5)) :> ILight)
    ]

    let vp = ViewPlane(width, height, 3.0 / float height, 64, 1.0) // Ortho config
    let vp = ViewPlane(width, height, 1.0 / float height, 64, 1.0) // Persp config
    let r = 3.0
    let theta = 110.0 / 180.0 * 3.1416
    let phi = 45.0 / 180.0 * 3.1416
    let roll = 0.0 / 180.0 * 3.1416
    let lookat = Vec3(0.0, -0.6, -1.5)
    let eyepoint = Vec3(r * Math.Sin(theta) * Math.Sin(phi), r * Math.Cos(phi), r * Math.Cos(theta) * Math.Sin(phi)) + lookat
    let camera = PinholeCamera(eyepoint, lookat, Vec3(Math.Sin(roll), Math.Cos(roll), 0.0), 1.0)
    let camera2 = OrthographicCamera(eyepoint, lookat, Vec3(Math.Sin(roll), Math.Cos(roll), 0.0))
    let world = World(vp, Vec3.Zero, hitableList, lightList, camera :> ICamera)

    Application.EnableVisualStyles()
    Application.SetCompatibleTextRenderingDefault false
    use form = new RayTracerMainGUI(world)
    Application.Run(form)
    0
