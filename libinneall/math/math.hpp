#pragma once

#include <cmath>

namespace inl {

static constexpr float PI = 3.14159265359f;
static constexpr float EPSILON = 1e-6f;

inline float to_degrees(float radian) { return radian * (180 / PI); }
inline float to_radians(float degrees) { return degrees * (PI / 180); }
inline float clamp_to_zero(float value) { return (std::fabs(value) < EPSILON) ? 0.0f : value; }

float clamp(float value, float min, float max);

} // namespace inl
