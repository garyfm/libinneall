#include "math/vector3.hpp"
#include <libinneall/camera.hpp>

namespace inl {

Camera::Camera(Vector3 position, Vector3 world_up, Vector3 front, float yaw, float pitch)
    : m_position { position }
    , m_front { front }
    , m_world_up { world_up }
    , m_right { normalise(cross(m_front, m_world_up)) }
    , m_up { normalise(cross(m_right, m_front)) }
    , m_yaw { yaw }
    , m_pitch { pitch } { }

void Camera::move(Direction dir, float velocity) {
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

void Camera::rotate(float xoffset, float yoffset) {

    m_yaw += xoffset;
    m_pitch += yoffset;

    m_pitch = clamp(m_pitch, -89.0f, 89.0f);

    Vector3 direction {};
    direction.x = cosf(to_radians(m_yaw)) * cosf(to_radians(m_pitch));
    direction.y = sinf(to_radians(m_pitch));
    direction.z = sinf(to_radians(m_yaw)) * cosf(to_radians(m_pitch));

    m_front = normalise(direction);

    // Recalcualte right and up based on new front
    m_right = normalise(cross(m_front, m_world_up));
    m_up = normalise(cross(m_right, m_front));
}

}
