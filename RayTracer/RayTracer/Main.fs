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
    Application.EnableVisualStyles()
    Application.SetCompatibleTextRenderingDefault false
    use form = new RayTracerMainGUI(world)
    Application.Run(form)
    0
