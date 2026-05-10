#include "scene.h"
#include "game.h"
#include "graphics.h"
#include "window.h"
#include "ui.h"

// MAIN MANU
void MainMenuScene::init() {
    m_PlayButton = new Button(COLOR_BLUE);
    m_PlayButton->m_Label.m_Text = "PLAY";
    m_PlayButton->m_Size = {200, 60};

    m_NameGameLabel = new Label("Ping Pong");
    m_NameGameLabel->m_FontSize = 60;

    m_ScreenTexture = App()->Graphics()->loadTexture("client/assets/textures/screen.png");

    App()->UI()->add("Play", m_PlayButton);
    App()->UI()->add("Name", m_NameGameLabel);
    App()->m_BackgroundColor = shared::ColorRGBA::fromHex(0x181818FF);
}

void MainMenuScene::update() {
    if (m_PlayButton->m_LeftClickPressed) {
        App()->setScene(new GameScene);
        return;
    }
}

void MainMenuScene::renderBackground() {
    auto graphics = App()->Graphics();
    auto s = App()->Window()->getScreenSize();

    float time = App()->Window()->getTime();

    float scale = 1.0f + sin(time * 1.5f) * 0.02f;

    float w = s.x * scale;
    float h = s.y * scale;

    float cx = s.x * 0.5f;
    float cy = s.y * 0.5f;

    graphics->drawTexture(
        m_ScreenTexture,
        {0, 0, (float)m_ScreenTexture.width, (float)m_ScreenTexture.height},
        {cx, cy, w, h},
        {w * 0.5f, h * 0.5f},
        30,
        {255, 255, 255, 100}
    );
}

void MainMenuScene::render() {
    shared::Vec2 s = App()->Window()->getScreenSize();

    renderBackground();

    m_PlayButton->m_Position = {
        (s.x - m_PlayButton->m_Size.x) / 2.0f,
        (s.y - m_PlayButton->m_Size.y) / 2.0f
    };

    m_NameGameLabel->m_Position = {s.x / 2, 60};

    m_PlayButton->draw();
    m_NameGameLabel->draw();
}

void MainMenuScene::cleanup() {
    App()->UI()->remove("Play");
    App()->UI()->remove("Name");

    App()->Graphics()->unloadTexture(m_ScreenTexture);
}

// GAME
void GameScene::init() {
    m_Game = new GameController();
    m_Game->init(960, 540);

    App()->Camera()->setTarget(m_Game->getFieldCenter());
    App()->Camera()->onResize(App()->Window()->getScreenSize());
    App()->m_BackgroundColor = shared::ColorRGBA::fromHex(0x1E1E1E00);
}

void GameScene::update() {
    if (App()->Window()->isKeyPressed(KCode::ESCAPE)) {
        App()->setScene(new MainMenuScene);
        return;
    }

    if (App()->Window()->isWindowResized()) {
        App()->Camera()->setTarget(m_Game->getFieldCenter());
    }

    m_Game->update();
}

void GameScene::render() {
    m_Game->render();
}

void GameScene::cleanup() {
    m_Game->cleanup();
    delete m_Game;
}