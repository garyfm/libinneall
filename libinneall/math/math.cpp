#include <libinneall/math/math.hpp>

namespace inl {

float clamp(float value, float min, float max) {
    if (value < min) {
        return min;
    }
    if (value > max) {
        return max;
    }
    return value;
}

float abs(float value) {
    if (value < 0) {
        return -value;
    }

    return value;
}

int abs(int value) {
    if (value < 0) {
        return -value;
    }

    return value;
}

} // namespace inl
