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
meson test -C build -v
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
- [x] Camera
- [x] Load images
- [x] Textures

Stage 2 - 3D mesh loader
- [x] OBJ format
- [x] Load mesh
- [x] Handle duplicated vertices 
- [ ] Load materials

Stage 3 - Lighting
- [x] Basic Phong Lighting
- [ ] Materials
- [ ] Light casters

Misc:
- [x] Error handing
- [x] Logging
- [x] Unique Resource
- [x] Setup testing
- [ ] Static String
- [ ] Add platform layer (Remove GLFW)
- [ ] Remove GLAD
