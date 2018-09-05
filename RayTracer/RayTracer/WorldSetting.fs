namespace RayTracer

open System
open Eto.Forms
open Eto.Drawing

type WorldSetting () as this =
    inherit WorldSettingBase()
    do
        base.InitializeComponent()
