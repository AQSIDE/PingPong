#include "engine.h"
#include "graphics.h"
#include "window.h"
#include "ui.h"

static AppContext* g_AppInstance = nullptr;

void AppContext::init(IGraphics* g, IWindow* w, ICamera* c, const WindowConfig& config) {
    // CREATE
    m_Graphics = g;
    m_Window = w;
    m_Camera = c;
    m_Renderer = new Renderer();
    m_UIManager = new UIManager();

    // INIT
    m_Window->createWindow(config);
    m_DefaultUISettings.textFont = m_Graphics->getDefaultFont();
    m_ScreenSize = m_Window->getScreenSize();
}

AppContext::~AppContext() {
    if (m_CurrentScene != nullptr) {
        m_CurrentScene->cleanup();
        delete m_CurrentScene;
    }

    delete m_Graphics;
    delete m_Window;
    delete m_Renderer;
    delete m_UIManager;
}

void AppContext::update() {
    m_Window->setMouseCursor(CursorType::DEFAULT);

    m_FrameData.deltaTime = m_Window->getDeltaTime();
    m_Camera->update();

    m_MousePos = m_Window->getMousePosition();
    m_MouseWorldPos = m_Window->getMouseWorldPosition(m_Camera->getData());

    if (m_Window->isWindowResized()) {
        m_ScreenSize = m_Window->getScreenSize();
        m_Camera->onResize(m_ScreenSize);
    }

    m_MouseActions.isLeftMouseButtonPressed = m_Window->isMouseButtonPressed(MCode::LEFT);
    m_MouseActions.isLeftMouseButtonReleased = m_Window->isMouseButtonReleased(MCode::LEFT);

    // UPDATE ALL
    m_FrameData.lastUpdateFrame++;

    m_UIManager->update();
    if (m_CurrentScene != nullptr) m_CurrentScene->update();
}

void AppContext::render() {
    m_FrameData.lastRenderFrame++;

    m_Graphics->beginDrawing();
    m_Graphics->clearColor(m_BackgroundColor);

    if (m_CurrentScene != nullptr)
        m_CurrentScene->render();

    m_Renderer->draw();
    m_Graphics->endDrawing();
}

const MouseActions &AppContext::getMouseActions() const {
    return m_MouseActions;
}

const DefaultUISettings &AppContext::getUISettings() const {
    return m_DefaultUISettings;
}

void AppContext::setUISettings(const DefaultUISettings &s) {
    m_Graphics->unloadFont(m_DefaultUISettings.textFont);

    m_DefaultUISettings = s;
    m_Graphics->setTextureFilter(m_DefaultUISettings.textFont.texture, FilterMode::BILINEAR);
}

void AppContext::setScene(IScene *s) {
    if (m_CurrentScene != nullptr) {
        m_CurrentScene->cleanup();
        delete m_CurrentScene;
    }

    m_CurrentScene = s;
    if (m_CurrentScene != nullptr) {
        m_CurrentScene->init();
    }
}

const FrameData &AppContext::getFrame() const {
    return m_FrameData;
}

// COMPONENTS
ICamera *AppContext::Camera() {
    return m_Camera;
}

Renderer *AppContext::Render() {
    return m_Renderer;
}

UIManager *AppContext::UI() {
    return m_UIManager;
}

IGraphics *AppContext::Graphics() {
    return m_Graphics;
}

IWindow *AppContext::Window() {
    return m_Window;
}

AppContext* App() {
    if (g_AppInstance == nullptr) {
        g_AppInstance = new AppContext;
    }
    return g_AppInstance;
}
