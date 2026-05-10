#pragma once

#include "base.h"

#include "transform.h"
#include "renderer.h"
#include "camera.h"
#include "entity.h"
#include "window.h"

const float TARGET_RATE = 1.0f / 60.0f;

class UIManager;
class IWindow;
class IGraphics;
class Renderer;

class IScene {
    public:

    virtual ~IScene() {}

    virtual void init() = 0;
    virtual void update() = 0;
    virtual void render() = 0;
    virtual void cleanup() = 0;
};

struct MouseActions {
    bool isLeftMouseButtonPressed = false;
    bool isLeftMouseButtonReleased = false;
};

struct FrameData {
    float deltaTime = 0;
    unsigned long lastRenderFrame = 0;
    unsigned long lastUpdateFrame = 0;
};

struct DefaultUISettings {
    shared::ColorRGBA textMainColor;
    shared::ColorRGBA textSecondaryColor;
    int textFontSize;
    FontHandle textFont;
};

class AppContext {
    // COMPONENTS
    ICamera* m_Camera = {};
    UIManager* m_UIManager = {};
    Renderer* m_Renderer = {};
    IGraphics* m_Graphics = {};
    IWindow* m_Window = {};

    // DATA
    shared::Vec2 m_ScreenSize = {0};
    shared::Vec2 m_MousePos = {0};
    shared::Vec2 m_MouseWorldPos = {0};
    MouseActions m_MouseActions = {};
    FrameData m_FrameData = {};
    DefaultUISettings m_DefaultUISettings = {};
    IScene* m_CurrentScene = nullptr;

public:
    shared::ColorRGBA m_BackgroundColor = COLOR_BLACK;

    ~AppContext();

    void init(IGraphics* g, IWindow* w, ICamera* c, const WindowConfig& config = {});
    void update();
    void render();

    const DefaultUISettings& getUISettings() const;
    void setUISettings(const DefaultUISettings& s);
    void setScene(IScene* s);
    const FrameData& getFrame() const;
    const MouseActions& getMouseActions() const;

    // COMPONENTS
    Renderer* Render();
    UIManager* UI();
    ICamera* Camera();
    IGraphics* Graphics();
    IWindow* Window();
};

AppContext* App();