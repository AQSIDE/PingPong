#pragma once

#include "engine.h"

class UIElement {
public:
    shared::Vec2 m_Position;
    shared::Vec2 m_Size;

    bool m_IsHovered;
    bool m_LeftClickReleased;
    bool m_LeftClickPressed;

    bool m_IsActive;
    bool m_IsVisible;

    UIElement(shared::Vec2 pos = {0}, shared::Vec2 size = {100, 100}) : m_Position(pos), m_Size(size), m_IsActive(true), m_IsVisible(true) {}

    virtual ~UIElement() {}

    virtual void update();
    virtual void draw() = 0;

    shared::Rect getRect() const;

    void setRect(shared::Rect rect);
};

class UIManager {
public:
    std::map<std::string, UIElement*> elements;
    void add(std::string name, UIElement* el) {
        elements[name] = el;
    }

    void update() {
        for (auto const& [name, el] : elements) {
            if (el->m_IsActive) el->update();
        }
    }

    void draw() {
        for (auto const& [name, el] : elements) {
            if (el->m_IsActive) el->draw();
        }
    }

    UIElement* getElement(std::string name) { return elements[name]; }

    template<typename T>
    T* get(const std::string& name) {
        UIElement* element = getElement(name);

        if (!element) return nullptr;
        return dynamic_cast<T*>(element);
    }

    void remove(const std::string& name) {
        auto it = elements.find(name);
        if (it != elements.end()) {
            delete it->second;
            elements.erase(it);
        }
    }

    ~UIManager() {
        for (auto const& [name, el] : elements) delete el;
        elements.clear();
    }
};

class Panel : public UIElement {
public:
    shared::ColorRGBA m_HoverColor;
    shared::ColorRGBA m_Color;

    Panel(shared::Vec2 size = {100, 100}, shared::ColorRGBA color = COLOR_WHITE) : UIElement({0}, size), m_Color(color), m_HoverColor(shared::ColorRGBA::brightness(color, -0.2f)) {}
    Panel(shared::ColorRGBA color = COLOR_WHITE) : m_Color(color), m_HoverColor(shared::ColorRGBA::brightness(color, -0.2f)) {}

    void update() override;
    void draw() override;
};

class InputField : public UIElement {
public:
    shared::ColorRGBA m_HoverColor;
    shared::ColorRGBA m_SelectedColor;
    shared::ColorRGBA m_Color;
    bool m_Selected;

    std::string m_Text;
    std::string m_Placeholder;
    int m_MaxLetters;

    InputField(shared::ColorRGBA color = COLOR_WHITE) :  m_MaxLetters(20), m_Color(color), m_HoverColor(shared::ColorRGBA::brightness(color, -0.2f)), m_SelectedColor(shared::ColorRGBA::brightness(color, 0.05f)) {}

    void update() override;
    void draw() override;
};

class Label : public UIElement {
public:
    std::string m_Text;
    shared::Vec2 m_Pivot;
    shared::ColorRGBA m_Color;
    int m_FontSize;

    Label(std::string text = "TEXT", int fontSize = 20, shared::Vec2 pivot = { 0.5f, 0.5f }, shared::ColorRGBA color = COLOR_WHITE) : m_Color(color), m_Pivot(pivot), m_FontSize(fontSize), m_Text(text) {}

    void update() override;
    void draw() override;
};

struct ButtonStyle {
    shared::ColorRGBA m_Color = COLOR_BLUE;
    shared::ColorRGBA m_HoverColor = COLOR_BLUE;
    shared::ColorRGBA m_ActiveColor = COLOR_BLUE;

    shared::Vec2 m_HoverSize;
    shared::Vec2 m_ActiveSize;
};

class Button : public UIElement {
public:
    Label m_Label;
    ButtonStyle m_Style;
    shared::ColorRGBA m_Color;
    shared::ColorRGBA m_HoverColor;

    Button(shared::ColorRGBA color = COLOR_BLUE) : m_Color(color), m_HoverColor(shared::ColorRGBA::brightness(color, -0.2f)), m_Label("TEXT") {}

    void update() override;
    void draw() override;
};