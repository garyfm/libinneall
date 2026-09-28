#pragma once

#include <libinneall/base/array.hpp>
#include <libinneall/base/utility.hpp>
#include <libinneall/renderer/mesh.hpp>
#include <libinneall/renderer/model.hpp>
#include <libinneall/renderer/texture.hpp>

namespace inl {

class AssetStore {
public:
    AssetStore() = default;

    INL_DEL_COPY_MOVE(AssetStore);

    Mesh* push_mesh();
    Texture* push_texture();
    Model* push_model();

    Span<Model> models() { return { m_model_store.data(), m_model_store_head }; };

private:
    static constexpr size_t STORE_SIZE = 10;

    // TODO: These should be pools
    Array<Mesh, STORE_SIZE> m_mesh_store {};
    size_t m_mesh_store_head {};
    Array<Texture, STORE_SIZE + 10> m_texture_store {};
    size_t m_texture_store_head {};
    Array<Model, STORE_SIZE> m_model_store {};
    size_t m_model_store_head {};
};

} // namespace inl
