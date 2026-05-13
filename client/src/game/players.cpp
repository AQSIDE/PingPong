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

    float v1 = p1.m_Transform.m_Vel.y;
    float v2 = p2.m_Transform.m_Vel.y;

    processInput(dt, speed, v1, v2);

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

void PlayersController::processInput(float dt, float speed, float &v1, float &v2) {
    auto* win = App()->Window();
    PlayMode pm = m_Game->getPlayMode();

    if (win->isKeyDown(KCode::W)) {
        v1 -= speed * dt;
    }
    if (win->isKeyDown(KCode::S)) {
        v1 += speed * dt;
    }

    if (pm == PlayMode::TOGETHER) {
        if (win->isKeyDown(KCode::UP)) {
            v2 -= speed * dt;
        }
        if (win->isKeyDown(KCode::DOWN)) {
            v2 += speed * dt;
        }
    }
    else if (pm == PlayMode::AI) {
        auto& ball = m_Game->getBall();
        auto& p2 = m_Game->getP2();

        float ballY = ball.m_Transform.m_Pos.y;
        float paddleY = p2.m_Transform.m_Pos.y;
        float targetY = ballY;

        if (ball.m_Transform.m_Vel.x > 0) {
            float distanceX = p2.m_Transform.m_Pos.x - ball.m_Transform.m_Pos.x;
            float timeToReach = distanceX / ball.m_Transform.m_Vel.x;

            targetY = ball.m_Transform.m_Pos.y + ball.m_Transform.m_Vel.y * timeToReach;
        }
        else {
            targetY = m_Game->getFieldSize().y / 2.0f;
        }

        float deadZone = 5.0f;
        if (targetY < paddleY - deadZone) {
            v2 -= speed * dt;
        }
        else if (targetY > paddleY + deadZone) {
            v2 += speed * dt;
        }
    }
}
