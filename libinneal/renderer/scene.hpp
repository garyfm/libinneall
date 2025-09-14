#pragma once

#include <libinneal/renderer/model.hpp>

#include <vector>

namespace inl {

struct Scene {
    std::vector<Model const*> models;
};

} // namespace inl
