#pragma once

#include "graphics.h"
#include "window.h"

class RaylibGraphics : public IGraphics {
public:
    virtual ~RaylibGraphics() {}

    FontHandle getDefaultFont() override;
    void setTextureFilter(TextureHandle &tex, FilterMode filter) override;
    shared::Vec2 measureText(FontHandle &font, const std::string &text, float fontSize, float spacing = 1.0f) override;
    FontHandle loadFont(const std::string& path, int fontSize) override;
    void unloadFont(FontHandle &f) override;
    TextureHandle loadTexture(const std::string& path) override;
    void unloadTexture(TextureHandle &t) override;
    void unloadRenderTexture(RenderTextureHandle &handle) override;
    void unloadShader(ShaderHandle &shader) override;
    ShaderHandle loadShader(const std::string &fragmentPath, const std::string &vertexPath) override;
    RenderTextureHandle createRenderTexture(int width, int height) override;
    void beginDrawing() override;
    void endDrawing() override;
    void clearColor(const shared::ColorRGBA &color) override;
    void beginCamera(const CameraData& data) override;
    void endCamera() override;
    void drawRect(shared::Rect rect, shared::ColorRGBA color) override;
    void beginRenderTexture(const RenderTextureHandle &tex) override;
    void endRenderTexture() override;
    void beginShader(const ShaderHandle &shader) override;
    void endShader() override;
    void drawText(const FontHandle &font, const std::string &text, shared::Vec2 pos, float fontSize, float spacing = 1.0f, shared::ColorRGBA color = COLOR_WHITE) override;
    void drawCircle(shared::Vec2 pos, int radius, shared::ColorRGBA color) override;
    void drawTexture(const TextureHandle &t, shared::Rect source, shared::Rect dest, shared::Vec2 pivot, float rot, shared::ColorRGBA color) override;
    void drawTexture(const TextureHandle &tex, shared::Vec2 pos, shared::ColorRGBA color) override;
    void drawRectLines(shared::Rect rect, float thickness, shared::ColorRGBA color) override;
    void drawLine(shared::Vec2 start, shared::Vec2 end, float thickness, shared::ColorRGBA color) override;
};

class WindowRaylib : public IWindow {
public:
    virtual ~WindowRaylib() override;

    void createWindow(const WindowConfig& config) override;
    void closeWindow() override;
    shared::Vec2 getMousePosition() override;
    shared::Vec2 getMouseWorldPosition(const CameraData& data) override;
    float getDeltaTime() override;
    float getTime() override;
    bool isOpen() override;
    bool isWindowResized() override;
    shared::Vec2 getScreenSize() override;

    // KEYBOARD
    bool isKeyDown(KCode k) override;
    bool isKeyPressed(KCode k) override;
    bool isKeyReleased(KCode k) override;
    int getCharPressed() override;

    //MOUSE
    bool isMouseButtonDown(MCode m) override;
    bool isMouseButtonPressed(MCode m) override;
    bool isMouseButtonReleased(MCode m) override;

    // CURSOR
    void setMouseCursor(CursorType c) override;
    void hideCursor() override;
    void showCursor() override;
};