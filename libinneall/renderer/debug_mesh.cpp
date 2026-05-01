#include <libinneall/base/array.hpp>
#include <libinneall/renderer/debug_mesh.hpp>

namespace inl {

Mesh* debug_mesh_triangle() {
    static Array<VertexData, 36> debug_triangle_vertices { {
        { { -0.5f, -0.5f, 0.0f }, {}, {} },
        { { 0.5f, -0.5f, 0.0f }, {}, {} },
        { { 0.5f, 0.5f, 0.0f }, {}, {} },

    } };

    static MeshData mesh_data { { debug_triangle_vertices.begin(), debug_triangle_vertices.end() }, {} };
    static Mesh mesh {};
    mesh.create(mesh_data);

    return &mesh;
}

Mesh* debug_mesh_quad() {
    static Array<VertexData, 36> debug_quad_vertices { {
        // triangle 1
        { { -0.5f, -0.5f, 0.0f }, {}, {} },
        { { 0.5f, -0.5f, 0.0f }, {}, {} },
        { { 0.5f, 0.5f, 0.0f }, {}, {} },

        // triangle 2
        { { 0.5f, 0.5f, 0.0f }, {}, {} },
        { { -0.5f, 0.5f, 0.0f }, {}, {} },
        { { -0.5f, -0.5f, 0.0f }, {}, {} },
    } };

    static MeshData mesh_data { { debug_quad_vertices.begin(), debug_quad_vertices.end() }, {} };
    static Mesh mesh {};
    mesh.create(mesh_data);

    return &mesh;
}

Mesh* debug_mesh_cube() {
    static Array<VertexData, 36> debug_cube_vertices { {
        { { -0.5f, -0.5f, -0.5f }, {}, {} },
        { { 0.5f, -0.5f, -0.5f }, {}, {} },
        { { 0.5f, 0.5f, -0.5f }, {}, {} },
        { { 0.5f, 0.5f, -0.5f }, {}, {} },
        { { -0.5f, 0.5f, -0.5f }, {}, {} },
        { { -0.5f, -0.5f, -0.5f }, {}, {} },

        { { -0.5f, -0.5f, 0.5f }, {}, {} },
        { { 0.5f, -0.5f, 0.5f }, {}, {} },
        { { 0.5f, 0.5f, 0.5f }, {}, {} },
        { { 0.5f, 0.5f, 0.5f }, {}, {} },
        { { -0.5f, 0.5f, 0.5f }, {}, {} },
        { { -0.5f, -0.5f, 0.5f }, {}, {} },

        { { -0.5f, 0.5f, 0.5f }, {}, {} },
        { { -0.5f, 0.5f, -0.5f }, {}, {} },
        { { -0.5f, -0.5f, -0.5f }, {}, {} },
        { { -0.5f, -0.5f, -0.5f }, {}, {} },
        { { -0.5f, -0.5f, 0.5f }, {}, {} },
        { { -0.5f, 0.5f, 0.5f }, {}, {} },

        { { 0.5f, 0.5f, 0.5f }, {}, {} },
        { { 0.5f, 0.5f, -0.5f }, {}, {} },
        { { 0.5f, -0.5f, -0.5f }, {}, {} },
        { { 0.5f, -0.5f, -0.5f }, {}, {} },
        { { 0.5f, -0.5f, 0.5f }, {}, {} },
        { { 0.5f, 0.5f, 0.5f }, {}, {} },

        { { -0.5f, -0.5f, -0.5f }, {}, {} },
        { { 0.5f, -0.5f, -0.5f }, {}, {} },
        { { 0.5f, -0.5f, 0.5f }, {}, {} },
        { { 0.5f, -0.5f, 0.5f }, {}, {} },
        { { -0.5f, -0.5f, 0.5f }, {}, {} },
        { { -0.5f, -0.5f, -0.5f }, {}, {} },

        { { -0.5f, 0.5f, -0.5f }, {}, {} },
        { { 0.5f, 0.5f, -0.5f }, {}, {} },
        { { 0.5f, 0.5f, 0.5f }, {}, {} },
        { { 0.5f, 0.5f, 0.5f }, {}, {} },
        { { -0.5f, 0.5f, 0.5f }, {}, {} },
        { { -0.5f, 0.5f, -0.5f }, {}, {} },
    } };

    // TODO: Avoid the copy here
    static MeshData mesh_data { { debug_cube_vertices.begin(), debug_cube_vertices.end() }, {} };
    static Mesh mesh {};
    mesh.create(mesh_data);

    return &mesh;
}

Mesh* mesh_cubemap() {
    static Array<VertexData, 36> cubemap_vertices { {
        { { -1.0f, -1.0f, -1.0f }, {}, {} },
        { { 1.0f, -1.0f, -1.0f }, {}, {} },
        { { 1.0f, 1.0f, -1.0f }, {}, {} },
        { { 1.0f, 1.0f, -1.0f }, {}, {} },
        { { -1.0f, 1.0f, -1.0f }, {}, {} },
        { { -1.0f, -1.0f, -1.0f }, {}, {} },

        { { -1.0f, -1.0f, 1.0f }, {}, {} },
        { { 1.0f, -1.0f, 1.0f }, {}, {} },
        { { 1.0f, 1.0f, 1.0f }, {}, {} },
        { { 1.0f, 1.0f, 1.0f }, {}, {} },
        { { -1.0f, 1.0f, 1.0f }, {}, {} },
        { { -1.0f, -1.0f, 1.0f }, {}, {} },

        { { -1.0f, 1.0f, 1.0f }, {}, {} },
        { { -1.0f, 1.0f, -1.0f }, {}, {} },
        { { -1.0f, -1.0f, -1.0f }, {}, {} },
        { { -1.0f, -1.0f, -1.0f }, {}, {} },
        { { -1.0f, -1.0f, 1.0f }, {}, {} },
        { { -1.0f, 1.0f, 1.0f }, {}, {} },

        { { 1.0f, 1.0f, 1.0f }, {}, {} },
        { { 1.0f, 1.0f, -1.0f }, {}, {} },
        { { 1.0f, -1.0f, -1.0f }, {}, {} },
        { { 1.0f, -1.0f, -1.0f }, {}, {} },
        { { 1.0f, -1.0f, 1.0f }, {}, {} },
        { { 1.0f, 1.0f, 1.0f }, {}, {} },

        { { -1.0f, -1.0f, -1.0f }, {}, {} },
        { { 1.0f, -1.0f, -1.0f }, {}, {} },
        { { 1.0f, -1.0f, 1.0f }, {}, {} },
        { { 1.0f, -1.0f, 1.0f }, {}, {} },
        { { -1.0f, -1.0f, 1.0f }, {}, {} },
        { { -1.0f, -1.0f, -1.0f }, {}, {} },

        { { -1.0f, 1.0f, -1.0f }, {}, {} },
        { { 1.0f, 1.0f, -1.0f }, {}, {} },
        { { 1.0f, 1.0f, 1.0f }, {}, {} },
        { { 1.0f, 1.0f, 1.0f }, {}, {} },
        { { -1.0f, 1.0f, 1.0f }, {}, {} },
        { { -1.0f, 1.0f, -1.0f }, {}, {} },
    } };

    // TODO: Avoid the copy here
    static MeshData mesh_data { { cubemap_vertices.begin(), cubemap_vertices.end() }, {} };
    static Mesh mesh {};
    mesh.create(mesh_data);

    return &mesh;
}
} // namespace inl
