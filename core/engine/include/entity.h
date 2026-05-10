#pragma once

#include "transform.h"
#include "renderer.h"
#include "base.h"

class IEntity {
public:
    Transform2D m_Transform;
    RenderContext m_RenderContext;
    std::set<std::string> m_Tags;

    SpriteAnimation *m_Animation;

    IEntity(SpriteAnimation *animation = nullptr)
        : m_Animation(animation) {
    }

    virtual ~IEntity() {
        if (m_Animation) delete m_Animation;
    }

    void updateAnimation(float dt) {
        if (m_Animation) {
            m_Animation->update(dt);
            m_RenderContext.currentFrame = m_Animation->getCurrentFrame();
        }
    }

    void addFlag(const std::string &flag) {
        m_Tags.insert(flag);
    }

    bool hasFlag(const std::string &flag) {
        return m_Tags.count(flag) > 0;
    }
};

class IActor : public IEntity {
protected:
    int m_Health;

public:
    IActor(int hp = 10);

    void takeDamage(int damage) {
        if (isDead()) return;

        m_Health -= damage;
        if (m_Health <= 0) {
            m_Health = 0;
            onDead();
            return;
        }

        onHit();
    }

    bool isDead() { return m_Health <= 0; }
    int getHealth() { return m_Health; }

protected:
    virtual void onDead() = 0;
    virtual void onHit() = 0;
};
