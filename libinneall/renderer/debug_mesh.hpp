#pragma once

#include <libinneall/renderer/mesh.hpp>

namespace inl {

Mesh* debug_mesh_triangle();
Mesh* debug_mesh_quad();
Mesh* debug_mesh_cube();
// TODO: Move out of here
Mesh* mesh_cubemap();

}
