#include "def_raylib.h"
#include "scene.h"

int main() {
    auto* app = App();
    auto* graphics = new RaylibGraphics;
    auto* window = new WindowRaylib;

    CameraData data;
    data.offset = window->getScreenSize() * 0.5f;
    data.target = {0, 0};
    data.zoom = 1.0f;
    data.rotation = 0.0f;

    app->init(graphics, window, new CameraBase(data), {1280, 740, "PingPong"});
    app->setUISettings({
        COLOR_WHITE,
        COLOR_GRAY,
        20,
        graphics->loadFont("client/assets/fonts/NES.ttf", 92)
    });

    app->setScene(new MainMenuScene);

    while (window->isOpen()) {
        app->update();
        app->render();
    }

    delete app;
    return 0;
}