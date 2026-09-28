#include <libinneall/asset/asset_store.hpp>
#include <libinneall/base/assert.hpp>

namespace inl {

Mesh* AssetStore::push_mesh() {
    inl_assert(m_mesh_store_head < m_mesh_store.size(), "Mesh store exceeded");
    return &m_mesh_store[m_mesh_store_head++];
}
Texture* AssetStore::push_texture() {
    inl_assert(m_texture_store_head < m_texture_store.size(), "Texture store exceeded");
    return &m_texture_store[m_texture_store_head++];
};
Model* AssetStore::push_model() {
    inl_assert(m_model_store_head < m_model_store.size(), "Model store exceeded");
    return &m_model_store[m_model_store_head++];
};

} // namespace inl
