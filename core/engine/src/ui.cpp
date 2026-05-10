#include "ui.h"
#include "engine.h"
#include "graphics.h"
#include "window.h"

void UIElement::update() {
    shared::Rect rect = getRect();

    m_IsHovered = checkCollisionPointRect(App()->Window()->getMousePosition(), rect);

    m_LeftClickReleased = m_IsHovered && App()->getMouseActions().isLeftMouseButtonReleased;
    m_LeftClickPressed = m_IsHovered && App()->getMouseActions().isLeftMouseButtonPressed;
}

shared::Rect UIElement::getRect() const {
    return { m_Position.x, m_Position.y, m_Size.x, m_Size.y };
}

void UIElement::setRect(shared::Rect rect) {
    m_Position.x = rect.x;
    m_Position.y = rect.y;
    m_Size.x = rect.width;
    m_Size.y = rect.height;
}

void Panel::update() {
    UIElement::update();
}

void Panel::draw() {
    if (!m_IsVisible) return;

    App()->Graphics()->drawRect(getRect(), m_IsHovered ? m_HoverColor : m_Color);
}

void InputField::update() {
    if (!m_IsActive) return;
    UIElement::update();

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
    UIElement::update();

    if (m_IsHovered) {
        App()->Window()->setMouseCursor(CursorType::IBEAM);
    }
}

void Label::draw() {
    if (!m_IsVisible) return;

    auto font = App()->getUISettings().textFont;
    float spacing = 1.0f;

    float currentSize = (m_FontSize > 0) ? (float)m_FontSize : 20.0f;
    shared::Vec2 textSize = App()->Graphics()->measureText(font, m_Text.c_str(), currentSize, spacing);

    shared::Vec2 drawPos = {
        m_Position.x - textSize.x * m_Pivot.x,
        m_Position.y - textSize.y * m_Pivot.y,
    };

    App()->Graphics()->drawText(font, m_Text, drawPos, currentSize, spacing, m_Color);
}

void Button::update() {
    if (!m_IsActive) return;
    UIElement::update();

    if (m_IsHovered) {
        App()->Window()->setMouseCursor(CursorType::POINTING_HAND);
    }
}

void Button::draw() {
    if (!m_IsVisible) return;
    shared::ColorRGBA target = m_Color;
    if (m_IsHovered) {
        target = m_HoverColor;
    }

    auto rect = getRect();
    App()->Graphics()->drawRect(rect, target);

    if (!m_Label.m_Text.empty()) {
        m_Label.m_Position = { rect.x + rect.width / 2, rect.y + rect.height / 2 };
        m_Label.draw();
    }
}