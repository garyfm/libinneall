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
- [x] Basic Materials
- [x] Light casters

Stage 4 - Advanced OpenGL
- [x] GLSL Interface blocks
- [x] GLSL UBO's
- [x] CubeMaps

Implement when needed:
- [ ] Instancing
- [ ] Anti-Aliasing
- [ ] Normal Maps

Misc:
- [x] Error handing
- [x] Logging
- [x] Unique Resource
- [x] Setup testing
- [x] Debug meshs
- [x] Static String
- [x] Use static constructors
- [ ] Add platform layer (Remove GLFW)
- [ ] Remove GLAD

Removing STL:
Size: 5116096
- [x] std integral types
- [x] std::byte
- [x] std::array
- [x] std::span
- [x] std::string
- [x] std::string_view
- [x] std::optional
- [x] remove execptions
- [ ] std::vector
- [ ] std::print
- [ ] std::expected
- [ ] math
- [ ] std::unqiue_ptr
- [ ] file handling
- [ ] hashmap
