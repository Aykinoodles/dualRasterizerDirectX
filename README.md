# Dual Rasterizer

<img src="https://Aykinoodles.github.io/img/GP1.png"  width="800">

## About
In this project I made a software rasterizer and a hardware rasterizer that can be switched at runtime.
The point here was to learn how the rasterizer works and what it does in actuality as with DX11 it can be a bit of a black box.

This project contains a Software Rasterizer that mimics DirectX, supporting 3D meshes and different shading
techniques. It also includes DirectX 11 to hardware accelerate the rasterization progress and some shaders.

## Controls
**MOUSE:**
- (Camera) Rotate Yaw (RMB + Mouse Move X)
- (Camera) Rotate Pitch (RMB + Mouse Move Y)
- (Camera) Move (local) Forward/Backward (LMB + Mouse Move Y)
- (Camera) Rotate Yaw (LMB + Mouse Move X)
- (Camera) Move (world) Up/Down (LMB + RMB + Mouse Move Y)

**KEYBOARD:**
- (Camera) Move (local) Forward (Arrow Up) and (‘W’)
- (Camera) Move (local) Backward (Arrow Down) and (‘S’)
- (Camera) Move (local) Right (Arrow Right) and (‘D’)
- (Camera) Move (local) Left (Arrow Left) and (‘A’)
- (Camera) Boost Movement Speed (LSHIFT)

**RENDERING:**
- (Shared) Toggle between DirectX & Software Rasterizer (‘F1’)
- (Shared) Toggle Rotation (On/Off) (‘F2’)
- (Shared) Cycle Cull Modes (back-face, front-face, none) (‘F9’)
- (Shared) Toggle Uniform Clear Color (On/Off) (‘F10’)
- (Shared) Toggle Print FPS (On/Off) (‘F11’)
- (Hardware Only) Toggle FireFX mesh (On/Off) (‘F3’)
- (Hardware Only) Toggle between Texture Sampling States (point-linear-anisotropic) (‘F4’)
- (Software Only) Cycle Shading Mode (Combined/ObservedArea/Diffuse/Specular) (‘F5’)
- (Software Only) Toggle NormalMap (On/Off) (‘F6’)
- (Software Only) Toggle DepthBuffer Visualization (On/Off) (‘F7’)
- (Software Only) Toggle BoundingBox Visualization (On/Off) (‘F8’
