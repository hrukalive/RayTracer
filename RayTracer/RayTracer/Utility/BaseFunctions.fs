namespace RayTracer

module BaseFunctions = 
    open System
    open System.Drawing
    open BaseTypes
    
    let GetDiffuseDirection (rand : Random) =
        let p = 2.0 * Vec3(rand.NextDouble(), rand.NextDouble(), rand.NextDouble()) - Vec3.One
        (Vec3Ops.Normalize p) * rand.NextDouble()

    let Reflect (vin:Vec3) (normal : Vec3) = 
        vin - 2.0 * (Vec3Ops.Dot vin normal) * normal

    let Refract (vin : Vec3) (normal : Vec3) (niNo : float) : Vec3 option =
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
    
    let Shuffle (org : _[]) = 
        let rng = new Random()
        let arr = Array.copy org
        let max = (arr.Length - 1)
        let randomSwap (arr : _[]) i =
            let pos = rng.Next(max)
            let tmp = arr.[pos]
            arr.[pos] <- arr.[i]
            arr.[i] <- tmp
            arr
        [| 0 .. max |] |> Array.fold randomSwap arr
