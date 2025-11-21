#include "math/vector3.hpp"
#include <libinneall/camera.hpp>

namespace inl {

Camera::Camera(Vector3 position, Vector3 world_up, Vector3 front, float movement_speed)
    : m_position { position }
    , m_front { front }
    , m_world_up { world_up }
    , m_right { normalise(cross(m_front, m_world_up)) }
    , m_up { normalise(cross(m_right, m_front)) }
    , m_movement_speed { movement_speed } { }

void Camera::move(Direction dir, float delta_time) {
    float velocity = m_movement_speed * delta_time;
    switch (dir) {
    case Direction::Forward:
        m_position += m_front * velocity;
        break;
    case Direction::Backward:
        m_position -= m_front * velocity;
        break;
    case Direction::Left:
        m_position -= m_right * velocity;
        break;
    case Direction::Right:
        m_position += m_right * velocity;
        break;
    }
}

}
