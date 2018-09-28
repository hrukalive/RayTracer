namespace RayTracer

module Sampler = 
    open System
    open BaseTypes
    
    type ISampler = 
        abstract member SquareSample : unit -> (float * float)[]
        abstract member CircleSample : unit -> (float * float)[]
    
    type OneSample() = 
        class
            interface ISampler with
                override this.SquareSample() = [| (0.5, 0.5) |]
                override this.CircleSample() = [| (0.0, 0.0) |]
        end

    type MultiJittered(NumSample : int) = 
        class
            let n : int = int (Math.Sqrt(float NumSample))
            let subCellWidth = 1.0 / float NumSample
            let rand = Random()
            
            interface ISampler with
                override this.SquareSample() = 
                    let samplesX : float[] = Array.init NumSample (fun _ -> 0.0)
                    let samplesY : float[] = Array.init NumSample (fun _ -> 0.0)
                    for i = 0 to n - 1 do
                        for j = 0 to n - 1 do
                            samplesX.[i * n + j] <- float (i * n + j) * subCellWidth + rand.NextDouble() * subCellWidth
                            samplesY.[i * n + j] <- float (j * n + i) * subCellWidth + rand.NextDouble() * subCellWidth
                    for i = 0 to n - 1 do
                        for j = 0 to n - 1 do
                            let k = rand.Next(j, n)
                            let tmp = samplesX.[i * n + j]
                            samplesX.[i * n + j] <- samplesX.[i * n + k]
                            samplesX.[i * n + k] <- tmp
                    for i = 0 to n - 1 do
                        for j = 0 to n - 1 do
                            let k = rand.Next(j, n)
                            let tmp = samplesY.[j * n + i]
                            samplesY.[j * n + i] <- samplesY.[k * n + i]
                            samplesY.[k * n + i] <- tmp
                    (Array.zip samplesX samplesY)
                override this.CircleSample() = 
                    let diskSamplesX : float[] = Array.init NumSample (fun _ -> 0.0)
                    let diskSamplesY : float[] = Array.init NumSample (fun _ -> 0.0)
                    
                    for i = 0 to n - 1 do
                        for j = 0 to n - 1 do
                            let spx = 2.0 * (float (i * n + j) * subCellWidth + rand.NextDouble() * subCellWidth) - 1.0
                            let spy = 2.0 * (float (j * n + i) * subCellWidth + rand.NextDouble() * subCellWidth) - 1.0
                            if spx > -spy then
                                if spx > spy then
                                    let r = spx
                                    let phi = spy / spx * Math.PI / 4.0
                                    diskSamplesX.[i * n + j] <- r * Math.Cos(phi)
                                    diskSamplesY.[i * n + j] <- r * Math.Sin(phi)
                                else
                                    let r = spy
                                    let phi = (2.0 - spx / spy) * Math.PI / 4.0
                                    diskSamplesX.[i * n + j] <- r * Math.Cos(phi)
                                    diskSamplesY.[i * n + j] <- r * Math.Sin(phi)
                            else
                                if spx < spy then
                                    let r = -spx
                                    let phi = (4.0 + spy / spx) * Math.PI / 4.0
                                    diskSamplesX.[i * n + j] <- r * Math.Cos(phi)
                                    diskSamplesY.[i * n + j] <- r * Math.Sin(phi)
                                else
                                    let r = -spy
                                    if spy <> 0.0 then
                                        let phi = (6.0 - spx / spy) * Math.PI / 4.0
                                        diskSamplesX.[i * n + j] <- r * Math.Cos(phi)
                                        diskSamplesY.[i * n + j] <- r * Math.Sin(phi)
                                    else
                                        diskSamplesX.[i * n + j] <- r
                                        diskSamplesY.[i * n + j] <- 0.0
                    for i = 0 to n - 1 do
                        for j = 0 to n - 1 do
                            let k = rand.Next(j, n)
                            let tmp = diskSamplesX.[i * n + j]
                            diskSamplesX.[i * n + j] <- diskSamplesX.[i * n + k]
                            diskSamplesX.[i * n + k] <- tmp
                    for i = 0 to n - 1 do
                        for j = 0 to n - 1 do
                            let k = rand.Next(j, n)
                            let tmp = diskSamplesY.[j * n + i]
                            diskSamplesY.[j * n + i] <- diskSamplesY.[k * n + i]
                            diskSamplesY.[k * n + i] <- tmp

                    (Array.zip diskSamplesX diskSamplesY)
        end