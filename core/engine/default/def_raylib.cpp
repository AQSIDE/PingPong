#include "def_raylib.h"
#include "raylib.h"

template<typename T>
T* native(void* ptr) {
    return static_cast<T*>(ptr);
}

ShaderHandle wrapShader(const Shader& shader) {
    ShaderHandle s;

    s.native = new Shader(shader);

    return s;
}

TextureHandle wrapTexture(const Texture& rlTex) {
    TextureHandle t;

    t.width = rlTex.width;
    t.height = rlTex.height;
    t.native = new Texture(rlTex);

    return t;
}

RenderTextureHandle wrapRenderTexture(const RenderTexture& texture) {
    RenderTextureHandle r;

    auto* nativeRT = new RenderTexture(texture);

    r.handle = nativeRT;

    r.texture.width = nativeRT->texture.width;
    r.texture.height = nativeRT->texture.height;
    r.texture.native = &nativeRT->texture;

    return r;
}

FontHandle wrapFont(const Font& rlFont) {
    FontHandle f;

    auto* nativeFont = new Font(rlFont);

    f.native = nativeFont;

    f.baseSize = rlFont.baseSize;
    f.texture.width = nativeFont->texture.width;
    f.texture.height = nativeFont->texture.height;
    f.texture.native = &nativeFont->texture;

    return f;
}

Camera2D convertToRaylibCamera(const CameraData& data) {
    Camera2D c;

    c.target = {data.target.x, data.target.y};
    c.offset = {data.offset.x, data.offset.y};
    c.zoom = data.zoom;
    c.rotation = data.rotation;

    return c;
}

// --- RaylibGraphics Implementation ---
FontHandle RaylibGraphics::getDefaultFont() {
    return wrapFont(GetFontDefault());
}

void RaylibGraphics::setTextureFilter(TextureHandle &tex, FilterMode filter) {
    SetTextureFilter(*native<Texture2D>(tex.native), (int)filter);
}

shared::Vec2 RaylibGraphics::measureText(FontHandle &font, const std::string &text, float fontSize, float spacing) {
    Vector2 size = MeasureTextEx(*native<Font>(font.native), text.c_str(), fontSize, spacing);
    return { size.x, size.y };
}

RenderTextureHandle RaylibGraphics::createRenderTexture(int width, int height) {
    return wrapRenderTexture(LoadRenderTexture(width, height));
}

void RaylibGraphics::unloadRenderTexture(RenderTextureHandle &handle) {
    if (!handle.handle) return;

    auto rlTex = native<RenderTexture>(handle.handle);

    UnloadRenderTexture(*rlTex);

    handle.handle = nullptr;
    handle.texture.native = nullptr;
    delete rlTex;
}

ShaderHandle RaylibGraphics::loadShader(const std::string &fragmentPath, const std::string &vertexPath) {
    const char* vPath = vertexPath.empty() ? nullptr : vertexPath.c_str();
    const char* fPath = fragmentPath.empty() ? nullptr : fragmentPath.c_str();

    return wrapShader(LoadShader(vPath, fPath));
}

void RaylibGraphics::unloadShader(ShaderHandle &shader) {
    if (!shader.native) return;

    auto* rlShader = native<Shader>(shader.native);
    UnloadShader(*rlShader);

    shader.native = nullptr;
    delete rlShader;
}

FontHandle RaylibGraphics::loadFont(const std::string& path, int fontSize) {
    return wrapFont(LoadFontEx(path.c_str(), fontSize, nullptr, 0));
}

void RaylibGraphics::unloadFont(FontHandle &f) {
    if (!f.native) return;
    auto rlFont = native<Font>(f.native);

    UnloadFont(*rlFont);
    f.texture.native = nullptr;
    f.native = nullptr;

    delete rlFont;
}

TextureHandle RaylibGraphics::loadTexture(const std::string& path) {
    return wrapTexture(LoadTexture(path.c_str()));
}

void RaylibGraphics::unloadTexture(TextureHandle &t) {
    if (!t.native) return;
    auto rlTex = native<Texture>(t.native);

    UnloadTexture(*rlTex);
    t.native = nullptr;

    delete rlTex;
}

void RaylibGraphics::beginShader(const ShaderHandle &shader) {
    BeginShaderMode(*native<Shader>(shader.native));
}

void RaylibGraphics::endShader() {
    EndShaderMode();
}

void RaylibGraphics::beginRenderTexture(const RenderTextureHandle &tex) {
    BeginTextureMode(*native<RenderTexture>(tex.handle));
}

void RaylibGraphics::endRenderTexture() {
    EndTextureMode();
}

void RaylibGraphics::beginDrawing() {
    BeginDrawing();
}

void RaylibGraphics::endDrawing() {
    EndDrawing();
}

void RaylibGraphics::clearColor(const shared::ColorRGBA &color) {
    ClearBackground({ color.r, color.g, color.b, color.a });
}

void RaylibGraphics::endCamera() {
    EndMode2D();
}

void RaylibGraphics::drawRect(shared::Rect rect, shared::ColorRGBA color) {
    DrawRectangleRec({ rect.x, rect.y, rect.width, rect.height },
                     { color.r, color.g, color.b, color.a });
}

void RaylibGraphics::drawRectRounded(shared::Rect rect, float radius, int segments, shared::ColorRGBA color) {
    DrawRectangleRounded({ rect.x, rect.y, rect.width, rect.height }, radius, segments,{ color.r, color.g, color.b, color.a });
}

void RaylibGraphics::drawRectRoundedLines(shared::Rect rect, float thickness, float radius, int segments, shared::ColorRGBA color) {
    DrawRectangleRoundedLinesEx({ rect.x, rect.y, rect.width, rect.height }, radius, segments, thickness, { color.r, color.g, color.b, color.a });
}

void RaylibGraphics::drawText(const FontHandle &font, const std::string &text, shared::Vec2 pos, float fontSize, float spacing, shared::ColorRGBA color) {
    DrawTextEx(*native<Font>(font.native), text.c_str(), { pos.x, pos.y }, fontSize, spacing,
               { color.r, color.g, color.b, color.a });
}

void RaylibGraphics::drawCircle(shared::Vec2 pos, int radius, shared::ColorRGBA color) {
    DrawCircleV({ pos.x, pos.y }, (float)radius,
                { color.r, color.g, color.b, color.a });
}

void RaylibGraphics::drawTexture(const TextureHandle &t, shared::Rect source, shared::Rect dest, shared::Vec2 pivot, float rot, shared::ColorRGBA color) {
    DrawTexturePro(*native<Texture>(t.native),
        { source.x, source.y, source.width, source.height },
        { dest.x, dest.y, dest.width, dest.height },
        { pivot.x, pivot.y }, rot,
        { color.r, color.g, color.b, color.a });
}

void RaylibGraphics::drawTexture(const TextureHandle &tex, shared::Vec2 pos, shared::ColorRGBA color) {
    DrawTextureV(*native<Texture>(tex.native), {pos.x, pos.y}, { color.r, color.g, color.b, color.a });
}

void RaylibGraphics::beginCamera(const CameraData& data) {
    BeginMode2D(convertToRaylibCamera(data));
}

void RaylibGraphics::drawRectLines(shared::Rect rect, float thickness, shared::ColorRGBA color) {
    DrawRectangleLinesEx({ rect.x, rect.y, rect.width, rect.height }, thickness,
                         { color.r, color.g, color.b, color.a });
}

void RaylibGraphics::drawLine(shared::Vec2 start, shared::Vec2 end, float thickness, shared::ColorRGBA color) {
    DrawLineEx(
        { start.x, start.y },
        { end.x, end.y },
        thickness,
        { color.r, color.g, color.b, color.a }
    );
}

// --- WindowRaylib Implementation ---
void WindowRaylib::createWindow(const WindowConfig& config) {
    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_RESIZABLE);
    InitWindow(config.width, config.height, config.title.c_str());
    SetExitKey(KEY_NULL);
}

void WindowRaylib::closeWindow() {
    CloseWindow();
}

shared::Vec2 WindowRaylib::getMousePosition() {
    Vector2 m = GetMousePosition();
    return { m.x, m.y };
}

shared::Vec2 WindowRaylib::getMouseWorldPosition(const CameraData& data) {
    Vector2 m = GetScreenToWorld2D(GetMousePosition(), convertToRaylibCamera(data));
    return { m.x, m.y };
}

float WindowRaylib::getDeltaTime() {
    return GetFrameTime();
}

float WindowRaylib::getTime() {
    return (float)GetTime();
}

bool WindowRaylib::isWindowResized() {
    return IsWindowResized();
}

bool WindowRaylib::isOpen() {
    return !WindowShouldClose();
}

shared::Vec2 WindowRaylib::getScreenSize() {
    return { (float)GetScreenWidth(), (float)GetScreenHeight() };
}

int WindowRaylib::getCharPressed() {
    return GetCharPressed();
}

bool WindowRaylib::isKeyDown(KCode k) {
    return IsKeyDown((int)k);
}

bool WindowRaylib::isKeyPressed(KCode k) {
    return IsKeyPressed((int)k);
}

bool WindowRaylib::isKeyReleased(KCode k) {
    return IsKeyReleased((int)k);
}

bool WindowRaylib::isMouseButtonDown(MCode m) {
    return IsMouseButtonDown((int)m);
}

bool WindowRaylib::isMouseButtonPressed(MCode m) {
    return IsMouseButtonPressed((int)m);
}

bool WindowRaylib::isMouseButtonReleased(MCode m) {
    return IsMouseButtonReleased((int)m);
}

void WindowRaylib::hideCursor() {
    HideCursor();
}

void WindowRaylib::showCursor() {
    ShowCursor();
}

void WindowRaylib::setMouseCursor(CursorType c) {
    SetMouseCursor((int)c);
}

WindowRaylib::~WindowRaylib() {
    CloseWindow();
}