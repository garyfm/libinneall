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

    Camera(Vector3 position, Vector3 world_up, Vector3 front, float movement_speed);

    void move(Direction dir, float delta_time);
    void update();

    Matrix4 view_matrix() const { return Matrix4::create_look_at(m_position, m_position + m_front, m_up); }

private:
    Vector3 m_position {};
    Vector3 m_front {};
    Vector3 m_world_up {};
    Vector3 m_right {};
    Vector3 m_up {};

    float m_movement_speed {};
    float m_sensitivity {};
};

} // namespace
