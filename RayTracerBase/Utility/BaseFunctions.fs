namespace RayTracerBase

module BaseFunctions = 
    open System
    open Eto.Drawing
    open BaseTypes

    let Clamp min max (x : float) = 
        match x with
        | x when System.Double.IsNaN(x) -> 0.0
        | x when x < min -> min
        | x when x > max -> max
        | _ -> x

    let Vec3ToDrawingColor (col : Vec3) =
        let (r, g, b) = (col.X * 255.0, col.Y*255.0, col.Z * 255.0)
        let clamp x = Clamp 0.0 255.0 x
        let (cr, cg, cb) = (clamp r, clamp g, clamp b)
    
        Color.FromArgb(255, int cr, int cg, int cb)
        
    let Gamma (col:Vec3) = Vec3(Math.Sqrt(col.X), Math.Sqrt(col.Y), Math.Sqrt(col.Z))
