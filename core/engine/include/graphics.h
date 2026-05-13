#pragma once

#include "shared.h"
#include <string>
#include "camera.h"
#include "codes.h"

class IGraphics {
public:
    virtual ~IGraphics() {}

    // FUNC
    virtual FontHandle getDefaultFont() = 0;
    virtual void setTextureFilter(TextureHandle& tex, FilterMode filter) = 0;
    virtual shared::Vec2 measureText(FontHandle& font, const std::string& text, float fontSize, float spacing = 1.0f) = 0;

    // font
    virtual FontHandle loadFont(const std::string& path, int fontSize) = 0;
    virtual void unloadFont(FontHandle& f) = 0;

    // texture
    virtual TextureHandle loadTexture(const std::string& path) = 0;
    virtual void unloadTexture(TextureHandle& t) = 0;

    // shader
    virtual ShaderHandle loadShader(const std::string& fragmentPath, const std::string& vertexPath) = 0;
    virtual void unloadShader(ShaderHandle& shader) = 0;

    // renderTexture
    virtual RenderTextureHandle createRenderTexture(int width, int height) = 0;
    virtual void unloadRenderTexture(RenderTextureHandle& handle) = 0;

    // RENDER
    virtual void clearColor(const shared::ColorRGBA& color) = 0;

    virtual void beginDrawing() = 0;
    virtual void endDrawing() = 0;

    virtual void beginRenderTexture(const RenderTextureHandle& tex) = 0;
    virtual void endRenderTexture() = 0;

    virtual void beginShader(const ShaderHandle& shader) = 0;
    virtual void endShader() = 0;

    virtual void beginCamera(const CameraData& data) = 0;
    virtual void endCamera() = 0;

    // PRIMITIVES
    virtual void drawRect(shared::Rect rect, shared::ColorRGBA color) = 0;
    virtual void drawRectRounded(shared::Rect rect, float radius, int segments, shared::ColorRGBA color) = 0;
    virtual void drawText(const FontHandle& font, const std::string& text, shared::Vec2 pos, float fontSize, float spacing = 1.0f, shared::ColorRGBA color = COLOR_WHITE) = 0;
    virtual void drawCircle(shared::Vec2 pos, int radius, shared::ColorRGBA color) = 0;

    virtual void drawTexture(const TextureHandle& t, shared::Rect source, shared::Rect dest, shared::Vec2 pivot, float rot = 0.0f, shared::ColorRGBA color = COLOR_WHITE) = 0;
    virtual void drawTexture(const TextureHandle& tex, shared::Vec2 pos, shared::ColorRGBA color = COLOR_WHITE) = 0;

    virtual void drawRectLines(shared::Rect rect, float thickness, shared::ColorRGBA color) = 0;
    virtual void drawRectRoundedLines(shared::Rect rect, float thickness, float radius, int segments, shared::ColorRGBA color) = 0;
    virtual void drawLine(shared::Vec2 start, shared::Vec2 end, float thickness, shared::ColorRGBA color) = 0;
};
