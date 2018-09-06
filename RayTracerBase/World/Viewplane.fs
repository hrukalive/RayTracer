namespace RayTracerBase

module ViewPlane = 
    open System
    open Eto.Drawing
    open BaseTypes
    open BaseFunctions

    type ViewPlane = 
        struct
            val Width : int
            val Height : int
            val PixelSize : float
            val NumSamples : int
            val Gamma : float
            val InvGamma : float
            val RenderedData : Vec3[,] * int[,] * Object
            val RenderedImage : Bitmap
            new (width : int, height : int, pixelSize : float, numSamples : int, gamma : float) = {
                Width = width;
                Height = height;
                PixelSize = pixelSize;
                NumSamples = numSamples;
                Gamma = gamma;
                InvGamma = 1.0 / gamma;
                RenderedData = ((Array2D.init width height (fun _ _ -> Vec3.Zero)), (Array2D.init width height (fun _ _ -> 0)), new Object());
                RenderedImage = new Bitmap(Size(width, height), PixelFormat.Format24bppRgb)
            }
        end

    let ResetRenderedImage (viewplane : ViewPlane) = 
        let (imageArray, countArray, _) = viewplane.RenderedData
        for i in 0 .. imageArray.GetLength(0) - 1 do
            for j in 0 .. imageArray.GetLength(1) - 1 do
                imageArray.[i, j] <- Vec3.Zero
                countArray.[i, j] <- 0
                viewplane.RenderedImage.SetPixel(i, j, Color.FromRgb(0))
    
    let inline SetPixel (viewplane : ViewPlane) (x : int, y : int, color : Vec3) = 
        let (imageArray, countArray, _) = viewplane.RenderedData
        imageArray.[x, y] <- imageArray.[x, y] + color
        countArray.[x, y] <- countArray.[x, y] + 1
        viewplane.RenderedImage.SetPixel(x, y, Vec3ToDrawingColor (imageArray.[x, y] / float countArray.[x, y]))

    let inline GetRenderedImage (viewplane : ViewPlane) = viewplane.RenderedImage

    let inline GetRenderedImageLock (viewplane : ViewPlane) = 
        let (_, _, lock) = viewplane.RenderedData
        lock
