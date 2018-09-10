namespace RayTracer

open System
open System.Drawing
open BaseTypes
open BaseFunctions
open FreeImageAPI

type ViewPlane = 
    struct
        val Width : int
        val Height : int
        val PixelSize : float
        val NumSamples : int
        val SqrtNumSamples : int
        val Gamma : float
        val InvGamma : float
        val RenderedArray : Vec3[,]
        val RenderedCount : int[,]
        val RenderLock : Object
        val RenderedImage : Bitmap
        new (width : int, height : int, pixelSize : float, numSamples : int, gamma : float) = {
            Width = width;
            Height = height;
            PixelSize = pixelSize;
            NumSamples = numSamples;
            SqrtNumSamples = int (Math.Sqrt (float numSamples));
            Gamma = gamma;
            InvGamma = 1.0 / gamma;
            RenderedArray = (Array2D.init width height (fun _ _ -> Vec3.Zero));
            RenderedCount = (Array2D.init width height (fun _ _ -> 0));
            RenderLock = new Object();
            RenderedImage = new Bitmap(width, height)
        }
        member inline self.GetSamplePoint (x : int, y : int, s : int) = 
            (self.PixelSize * (float x - (float self.Width / 2.0) + ((float (s % self.SqrtNumSamples) + 0.5) / float self.SqrtNumSamples)),
             self.PixelSize * (float y - (float self.Height / 2.0) + ((float (s / self.SqrtNumSamples) + 0.5) / float self.SqrtNumSamples)))
        member inline self.ResetRenderedImage () = 
            for i in 0 .. self.RenderedArray.GetLength(0) - 1 do
                for j in 0 .. self.RenderedArray.GetLength(1) - 1 do
                    self.RenderedArray.[i, j] <- Vec3.Zero
                    self.RenderedCount.[i, j] <- 0
                    self.RenderedImage.SetPixel(i, j, Color.Black)
        member inline self.SetPixel (x : int, y : int, color : Vec3) = 
            self.RenderedArray.[x, y] <-self.RenderedArray.[x, y] + color
            self.RenderedCount.[x, y] <- self.RenderedCount.[x, y] + 1
            self.RenderedImage.SetPixel(x, y, Vec3ToDrawingColor (self.RenderedArray.[x, y] / float self.RenderedCount.[x, y]))
    end
