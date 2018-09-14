namespace RayTracer

module Sampler = 
    open System
    open BaseTypes
    
    type ISampler = 
        abstract member GenerateSample : unit -> unit
        abstract member Sample : int * int -> (float * float)
    
    type MultiJittered(NumSample : int, NumSet : int) = 
        class
            let samplesX : float[,] = Array2D.init NumSet NumSample (fun _ _ -> 0.0)
            let samplesY : float[,] = Array2D.init NumSet NumSample (fun _ _ -> 0.0)
            let n : int = int (Math.Sqrt(float NumSample))
            let subCellWidth = 1.0 / float NumSample
            let rand = Random()
            interface ISampler with
                override this.GenerateSample() = 
                    for p = 0 to NumSet - 1 do
                        for i = 0 to n - 1 do
                            for j = 0 to n - 1 do
                                samplesX.[p, i * n + j] <- float (i * n + j) * subCellWidth + rand.NextDouble() * subCellWidth
                                samplesY.[p, i * n + j] <- float (j * n + i) * subCellWidth + rand.NextDouble() * subCellWidth
                    for p = 0 to NumSet - 1 do
                        for i = 0 to n - 1 do
                            for j = 0 to n - 1 do
                                let k = rand.Next(j, n)
                                let tmp = samplesX.[p, i * n + j]
                                samplesX.[p, i * n + j] <- samplesX.[p, i * n + k]
                                samplesX.[p, i * n + k] <- tmp
                    for p = 0 to NumSet - 1 do
                        for i = 0 to n - 1 do
                            for j = 0 to n - 1 do
                                let k = rand.Next(j, n)
                                let tmp = samplesY.[p, j * n + i]
                                samplesY.[p, j * n + i] <- samplesY.[p, k * n + i]
                                samplesY.[p, k * n + i] <- tmp
                    0 |> ignore
                override this.Sample(set : int, sid : int) = (samplesX.[set, sid], samplesY.[set, sid])
        end