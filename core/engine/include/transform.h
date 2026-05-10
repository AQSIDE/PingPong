#pragma once

#include "shared.h"

constexpr shared::Vec2 BASE_SIZE = {30, 30};
constexpr shared::Vec2 BASE_PIVOT = {0.5f, 0.5f};

class Transform2D {
public:
    shared::Vec2 m_Pos;
    shared::Vec2 m_Size;
    float m_Rot;

    shared::Vec2 m_Vel;
    shared::Vec2 m_Pivot;

    Transform2D()
        : m_Pos({0}), m_Size(BASE_SIZE), m_Rot(0), m_Pivot(BASE_PIVOT), m_Vel({0}) {
    }

    void move(shared::Vec2 to) {
        m_Pos.x += to.x;
        m_Pos.y += to.y;
    }

    void addVelocity(shared::Vec2 vel) {
        m_Vel.x += vel.x;
        m_Vel.y += vel.y;
    }

    void rotate(float r) {
        m_Rot += r;

        if (m_Rot >= 360.0f) m_Rot -= 360.0f;
        if (m_Rot < 0.0f) m_Rot += 360.0f;
    }
};
