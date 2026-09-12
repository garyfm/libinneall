#include <libinneall/asset/asset_store.hpp>

namespace inl {

Mesh* AssetStore::push_mesh() { return &m_mesh_store[m_mesh_store_head++]; }
Texture* AssetStore::push_texture() { return &m_texture_store[m_texture_store_head++]; };
Model* AssetStore::push_model() { return &m_model_store[m_model_store_head++]; };

} // namespace inl
