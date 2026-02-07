#pragma once

#include <libinneall/math/matrix4.hpp>
#include <libinneall/math/vector3.hpp>

namespace inl {

struct CameraInitialSettings {
    Vector3 position;
    Vector3 world_up;
    Vector3 front;
    float yaw;
    float pitch;
    float fov;
    float z_near;
    float z_far;
};

class Camera {
public:
    enum class Direction {
        Forward,
        Backward,
        Left,
        Right,
    };

    Camera(CameraInitialSettings settings);

    void move(Direction dir, float delta_time);
    void rotate(float yaw, float pitch);
    void zoom(float zoom_value);

    Matrix4 view_matrix() const { return Matrix4::create_look_at(m_position, m_position + m_front, m_up); }
    Matrix4 perspective_matrix(float aspect) const {
        return Matrix4::create_perspective(to_radians(m_fov), aspect, m_z_near, m_z_far);
    }

    Vector3 position() const { return m_position; }
    Vector3 front() const { return m_front; }
    float yaw() const { return m_yaw; }
    float pitch() const { return m_pitch; }

private:
    static constexpr float min_zoom = 1.0f;
    static constexpr float max_zoom = 45.0f;

    Vector3 m_position {};
    Vector3 m_world_up {};
    Vector3 m_front {};

    Vector3 m_right {};
    Vector3 m_up {};

    float m_yaw {};
    float m_pitch {};

    float m_fov {};
    float m_z_near {};
    float m_z_far {};
};

} // namespace
