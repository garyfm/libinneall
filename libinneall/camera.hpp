#pragma once

#include <libinneall/math/matrix4.hpp>
#include <libinneall/math/vector3.hpp>

namespace inl {

class Camera {
public:
    enum class Direction {
        Forward,
        Backward,
        Left,
        Right,
    };

    Camera(Vector3 position, Vector3 world_up, Vector3 front, float yaw, float pitch);

    void move(Direction dir, float delta_time);
    void rotate(float yaw, float pitch);

    Matrix4 view_matrix() const { return Matrix4::create_look_at(m_position, m_position + m_front, m_up); }

    Vector3 position() const { return m_position; }
    float yaw() const { return m_yaw; }
    float pitch() const { return m_pitch; }

private:
    Vector3 m_position {};
    Vector3 m_front {};
    Vector3 m_world_up {};
    Vector3 m_right {};
    Vector3 m_up {};

    float m_yaw {};
    float m_pitch {};
};

} // namespace
