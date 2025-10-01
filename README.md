# LibInneall

A toy project to learn graphic programing

## Build

Generate the build files:
```bash
meson setup build
```

Compile:
```bash
meson compile -C build
```

Run:
```
./build/demo/game resources
```

## TODO

Stage 1 - 3D model loader
- [x] Logging
- [x] Unique Resource
- [x] Setup shader compilation
- [x] Render triangle
- [x] DSA vertex array/buffer
- [x] Element buffer
- [x] Mesh class
- [x] Renderer class
- [x] Error handing
- [x] Shader uniforms 
- [ ] Math
- [ ] Camera
- [ ] Model format
- [ ] Model loader

Stage 2
- [ ] Image format
- [ ] Textures
- [ ] Lighting

Misc:
- [ ] Static String
- [ ] Add platform layer (Remove GLFW)
- [ ] Remove GLAD
