#include "def_raylib.h"
#include "scene.h"

int main() {
    auto* app = App();
    auto* graphics = new RaylibGraphics;
    auto* window = new WindowRaylib;

    app->init(graphics, window, new CameraBase, {1280, 740, "PingPong"});
    app->setUISettings({
        COLOR_WHITE,
        COLOR_GRAY,
        20,
        graphics->loadFont("assets/fonts/NES.ttf", 92)
    });

    app->setScene(new MainMenuScene);

    while (window->isOpen()) {
        app->update();
        app->render();
    }

    delete app;
    return 0;
}