#pragma once

#include <math.h>
#include <stdint.h>

namespace inl {

static constexpr float PI = 3.14159265359f;
static constexpr float EPSILON = 1e-6f;

static constexpr int32_t MAX_INT32 = 2'147'483'647;
static constexpr int32_t MIN_INT32 = -2'147'483'648;
static constexpr uint32_t MAX_UINT32 = 4'294'967'295;

inline float to_degrees(float radian) { return radian * (180 / PI); }
inline float to_radians(float degrees) { return degrees * (PI / 180); }
inline float clamp_to_zero(float value) { return (std::fabs(value) < EPSILON) ? 0.0f : value; }

float clamp(float value, float min, float max);
float abs(float value);
int abs(int value);

} // namespace inl
