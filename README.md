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
```bash
./build/demo/game resources
```

Test:

```bash
build test -C build
```

## TODO

Stage 1 - Draw scene
- [x] Setup shader compilation
- [x] Shader uniforms 
- [x] Render triangle
- [x] DSA vertex array/buffer
- [x] Element buffer
- [x] Renderer class
- [x] Math
- [ ] Camera

Stage 2 - 3D model loader
- [ ] Model class
- [ ] Model format
- [ ] Model loader

Stage 3
- [ ] Image format
- [ ] Textures
- [ ] Lighting

Misc:
- [x] Error handing
- [x] Logging
- [x] Unique Resource
- [x] Setup testing
- [ ] Static String
- [ ] Add platform layer (Remove GLFW)
- [ ] Remove GLAD
