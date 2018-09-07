﻿namespace RayTracer

open System
open BaseTypes
open Material.MaterialFuncs

type Camera(position : Vec3, lookAt : Vec3, up : Vec3, vfov : float, aspect : float, r : float, focusDist : float) = 
    class
        let radius = r * 0.5
        let unitAngle = Math.PI / 180.0 * vfov
        let halfHeight = Math.Tan(unitAngle * 0.5)
        let halfWidth = aspect * halfHeight

        let w = Vec3Ops.Normalize (lookAt - position)
        let u = (Vec3Ops.Cross up w) |> Vec3Ops.Normalize
        let v = (Vec3Ops.Cross w u) |> Vec3Ops.Normalize

        let lowLeftCorner = position + w * focusDist - halfWidth * u * focusDist - halfHeight * v * focusDist
        let horizontal = 2.0 * halfWidth * u * focusDist
        let vertical = 2.0 * halfHeight * v * focusDist

        member this.CreateRay(u : float, v : float) = 
            let rand = System.Random(int (u * v * 9999999.0))
            let rd = radius * GetDiffuseDirection(rand)
            let offset = Vec3(rd.X * u + rd.Y * v)
            Ray(position + offset, lowLeftCorner + u * horizontal + v * vertical - position)
    end