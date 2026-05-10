#include "players.h"

#include "engine.h"
#include "game.h"
#include "window.h"

void PlayersController::init(GameController *game) {
    m_Game = game;
}

void PlayersController::update() {
    if (!m_Game) return;

    float dt = App()->getFrame().deltaTime;
    float speed = 8000.0f;
    float friction = 0.92f;

    auto& p1 = m_Game->getP1();
    auto& p2 = m_Game->getP2();
    auto* win = App()->Window();

    float v1 = p1.m_Transform.m_Vel.y;
    float v2 = p2.m_Transform.m_Vel.y;

    // INPUT
    if (win->isKeyDown(KCode::W)) {
        v1 -= speed * dt;
    }
    if (win->isKeyDown(KCode::S)) {
        v1 += speed * dt;
    }

    if (win->isKeyDown(KCode::UP)) {
        v2 -= speed * dt;
    }
    if (win->isKeyDown(KCode::DOWN)) {
        v2 += speed * dt;
    }

    v1 *= friction;
    v2 *= friction;

    // MOVE
    p1.m_Transform.m_Pos.y += v1 * dt;
    p2.m_Transform.m_Pos.y += v2 * dt;

    // COLLISION
    float h1 = p1.m_Transform.m_Size.y / 2.0f;
    float h2 = p2.m_Transform.m_Size.y / 2.0f;
    shared::Vec2 field = m_Game->getFieldSize();

    if (p1.m_Transform.m_Pos.y - h1 < 0) { p1.m_Transform.m_Pos.y = h1; v1 = 0; }
    if (p1.m_Transform.m_Pos.y + h1 > field.y) { p1.m_Transform.m_Pos.y = field.y - h1; v1 = 0; }

    if (p2.m_Transform.m_Pos.y - h2 < 0) { p2.m_Transform.m_Pos.y = h2; v2 = 0; }
    if (p2.m_Transform.m_Pos.y + h2 > field.y) { p2.m_Transform.m_Pos.y = field.y - h2; v2 = 0; }

    // ADD VEL
    p1.m_Transform.m_Vel.y = v1;
    p2.m_Transform.m_Vel.y = v2;
}