#include "ui.h"
#include "engine.h"
#include "graphics.h"
#include "window.h"

void updateState(UIElement* el) {
    if (!el->m_IsActive) return;

    shared::Rect rect = el->getRect();

    el->m_IsHovered = checkCollisionPointRect(App()->Window()->getMousePosition(), rect);

    el->m_LeftClickReleased = el->m_IsHovered && App()->getMouseActions().isLeftMouseButtonReleased;
    el->m_LeftClickPressed = el->m_IsHovered && App()->getMouseActions().isLeftMouseButtonPressed;
}

shared::Rect UIElement::getRect() const {
    return {m_Position.x, m_Position.y, 0, 0};
}

void Panel::setColor(shared::ColorRGBA color) {
    m_currentColor = color;
    m_Style.color = color;
}

void Panel::update() {
    if (!m_IsActive) return;

    updateState(this);

    shared::ColorRGBA targetColor = m_Style.color;
    float targetScale = 1.0f;

    if (m_IsHovered) {
        targetColor = m_Style.hoverColor;
        targetScale = m_Style.scaleFactor;
    }

    if (m_LeftClickPressed) {
        targetColor = m_Style.activeColor;
        targetScale = m_Style.scaleFactor;
    }

    float dt = App()->getFrame().deltaTime * 15.0f;
    dt = std::min(dt, 1.0f);

    m_currentColor = shared::ColorRGBA::lerp(m_currentColor, targetColor, dt);
    m_currentScale = m_currentScale + (targetScale - m_currentScale) * dt;
}

void Panel::draw() {
    if (!m_IsVisible) return;

    auto rect = getRect();

    if (m_currentScale != 1.0f) {
        float centerX = rect.x + rect.width / 2.0f;
        float centerY = rect.y + rect.height / 2.0f;
        rect.width *= m_currentScale;
        rect.height *= m_currentScale;
        rect.x = centerX - rect.width / 2.0f;
        rect.y = centerY - rect.height / 2.0f;
    }

    if (m_Style.borderRadius > 0.0f) {
        App()->Graphics()->drawRectRounded(rect, m_Style.borderRadius, m_Style.borderFragments, m_currentColor);

        if (m_Style.outlineThickness > 0.0f) {
            App()->Graphics()->drawRectRoundedLines(rect, m_Style.outlineThickness, m_Style.borderRadius, m_Style.borderFragments, m_Style.outlineColor);
        }
    }
    else {
        App()->Graphics()->drawRect(rect, m_currentColor);

        if (m_Style.outlineThickness > 0.0f) {
            App()->Graphics()->drawRectLines(rect, m_Style.outlineThickness, m_Style.outlineColor);
        }
    }
}

shared::Rect Panel::getRect() const {
    float w = m_Size.x * m_currentScale;
    float h = m_Size.y * m_currentScale;
    float x = m_Position.x + (m_Size.x - w) / 2.0f;
    float y = m_Position.y + (m_Size.y - h) / 2.0f;
    return {x, y, w, h};
}

void InputField::update() {
    if (!m_IsActive) return;
    updateState(this);

    if (m_IsHovered) {
        App()->Window()->setMouseCursor(CursorType::POINTING_HAND);
    }

    if (App()->getMouseActions().isLeftMouseButtonPressed) {
        m_Selected = m_IsHovered;
    }

    auto* win = App()->Window();

    if (m_Selected) {
        int c = win->getCharPressed();

        if (c > 0 && m_Text.size() <= m_MaxLetters) {
            m_Text += (char)c;
        }

        if (win->isKeyPressed(KCode::BACKSPACE) && !m_Text.empty()) {
            m_Text.pop_back();
        }
    }
}

void InputField::draw() {
    if (!m_IsVisible) return;

    shared::ColorRGBA targetColor = m_Selected ? m_SelectedColor : (m_IsHovered ? m_HoverColor : m_Color);
    auto rect = getRect();

    App()->Graphics()->drawRect(rect, targetColor);

    float paddingX = 15.0f;
    float textY = rect.y + (rect.height / 2.0f) - (14 / 2.0f);

    auto font = App()->getUISettings().textFont;

    if (m_Text.empty() && !m_Placeholder.empty()) {
        App()->Graphics()->drawText(font, m_Placeholder, {rect.x + paddingX, textY}, App()->getUISettings().textFontSize, 1, App()->getUISettings().textSecondaryColor);
    }
    else {
        App()->Graphics()->drawText(font, m_Text, {rect.x + paddingX, textY}, App()->getUISettings().textFontSize, 1, App()->getUISettings().textMainColor);
    }

    if (m_Selected) {
        float time = App()->Window()->getTime();
        if ((int)(time * 2.0f) % 2 == 0) {
            shared::Vec2 textSize = App()->Graphics()->measureText(font, m_Text, App()->getUISettings().textFontSize);
            float cursorX = rect.x + paddingX + textSize.x + 2;

            App()->Graphics()->drawRect({cursorX, rect.y + 10, 2, rect.height - 20}, App()->getUISettings().textMainColor);
        }
    }
}

void Label::update() {
    if (!m_IsActive) return;

    // if (m_IsHovered) {
    //     App()->Window()->setMouseCursor(CursorType::IBEAM);
    // }
}

void Label::draw() {
    if (!m_IsVisible || m_Text.empty()) return;

    float currentSize = (m_Style.fontSize > 0) ? (float)(m_Style.fontSize) : (float)App()->getUISettings().textFontSize;

    shared::Vec2 textSize = App()->Graphics()->measureText(m_Font, m_Text, currentSize, m_Style.spacing);

    shared::Vec2 drawPos = {
        m_Position.x - textSize.x * m_Pivot.x,
        m_Position.y - textSize.y * m_Pivot.y,
    };

    if (m_Style.shadowColor.a > 0) {
        shared::Vec2 shadowDrawPos = {
            drawPos.x + m_Style.shadowOffset.x,
            drawPos.y + m_Style.shadowOffset.y
        };

        App()->Graphics()->drawText(
            m_Font,
            m_Text,
            shadowDrawPos,
            currentSize,
            m_Style.spacing,
            m_Style.shadowColor
        );
    }

    App()->Graphics()->drawText(
        m_Font,
        m_Text,
        drawPos,
        currentSize,
        m_Style.spacing,
        m_Style.color
    );
}

void Button::update() {
    if (!m_IsActive) return;

    Panel::update();

    if (m_IsHovered) {
        App()->Window()->setMouseCursor(CursorType::POINTING_HAND);
    }

    // if (!m_Label.m_Text.empty()) {
    //     m_Label.update();
    // }
}

void Button::draw() {
    if (!m_IsVisible) return;

    Panel::draw();

    if (!m_Label.m_Text.empty()) {
        auto rect = Panel::getRect();

        m_Label.m_Position = { rect.x + rect.width / 2.0f, rect.y + rect.height / 2.0f };
        m_Label.draw();
    }
}