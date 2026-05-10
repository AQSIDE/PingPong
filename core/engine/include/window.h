#pragma once
#include "shared.h"
#include "camera.h"
#include "codes.h"

struct WindowConfig {
    int width = 640;
    int height = 480;
    std::string title = "Title";
};

class IWindow {
public:
    virtual ~IWindow() {}

    virtual void createWindow(const WindowConfig& config) = 0;
    virtual void closeWindow() = 0;
    virtual bool isOpen() = 0;
    virtual shared::Vec2 getMousePosition() = 0;
    virtual shared::Vec2 getMouseWorldPosition(const CameraData& data) = 0;
    virtual float getDeltaTime() = 0;
    virtual float getTime() = 0;
    virtual bool isWindowResized() = 0;
    virtual shared::Vec2 getScreenSize() = 0;

    // KEYBOARD
    virtual bool isKeyPressed(KCode k) = 0;
    virtual bool isKeyDown(KCode k) = 0;
    virtual bool isKeyReleased(KCode k) = 0;
    virtual int getCharPressed() = 0;

    // MOUSE
    virtual bool isMouseButtonDown(MCode m) = 0;
    virtual bool isMouseButtonPressed(MCode m) = 0;
    virtual bool isMouseButtonReleased(MCode m) = 0;

    // CURSOR
    virtual void setMouseCursor(CursorType c) = 0;
    virtual void hideCursor() = 0;
    virtual void showCursor() = 0;
};
