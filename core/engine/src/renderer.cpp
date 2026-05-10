#include "renderer.h"

#include "engine.h"
#include "graphics.h"

void Renderer::drawItem(Transform2D* tr, RenderContext* c) {
    if (c->texture == nullptr || tr == nullptr) return;

    shared::Rect source;
    if (c != nullptr && c->currentFrame.width > 0 && c->currentFrame.height > 0) {
        source = c->currentFrame.getRect();
    }
    else {
        source = { 0.0f, 0.0f, (float)c->texture->width, (float)c->texture->height };
    }

    if (c != nullptr) {
        if (c->flipX) source.width *= -1;
        if (c->flipY) source.height *= -1;
    }

    shared::Rect dest = { tr->m_Pos.x, tr->m_Pos.y, tr->m_Size.x, tr->m_Size.y };
    shared::Vec2 pivot = {
        dest.width * tr->m_Pivot.x,
        dest.height * tr->m_Pivot.y
    };

    App()->Graphics()->drawTexture(*c->texture, source, dest, pivot, tr->m_Rot, c->color);

    // DEBUG
    if (m_RenderDebug) {
        shared::Rect bounds = {
            tr->m_Pos.x - pivot.x,
            tr->m_Pos.y - pivot.y,
            tr->m_Size.x,
            tr->m_Size.y
        };

        App()->Graphics()->drawRectLines(bounds, 1, COLOR_GREEN);
        App()->Graphics()->drawCircle(tr->m_Pos, 2, COLOR_RED);
    }
}

void Renderer::add(Transform2D* tr, RenderContext* c) {
    if (c->texture == nullptr || tr == nullptr) return;

    m_Items.push_back({ tr, c });
}

void Renderer::draw() {
    if (m_Items.empty()) return;

    std::sort(m_Items.begin(), m_Items.end(),
        [](const RenderItem& a, const RenderItem& b) {return a.y() < b.y();});

    for (auto& i : m_Items) {
        drawItem(i.transform, i.context);
    }

    m_Items.clear();
}