#include "scene.h"
#include "game.h"
#include "graphics.h"
#include "window.h"
#include "ui.h"

static PlayMode selectedPlayMode = PlayMode::TOGETHER;

// MAIN MANU
void MainMenuScene::init() {
    // UI

    float fontSize = 25;
    shared::Vec2 size = {200, 60};

    m_PlayButton = new Button();
    m_PlayButton->m_Label.m_Text = "PLAY";
    m_PlayButton->m_Size = size;
    m_PlayButton->m_Label.m_Style.fontSize = fontSize;

    m_SettingsButton = new Button();
    m_SettingsButton->m_Label.m_Text = "SETTINGS";
    m_SettingsButton->m_Size = size;
    m_SettingsButton->m_Label.m_Style.fontSize = fontSize;

    m_ExitButton = new Button();
    m_ExitButton->m_Label.m_Text = "EXIT";
    m_ExitButton->m_Size = size;
    m_ExitButton->m_Label.m_Style.fontSize = fontSize;

    m_TogetherModeButton = new Button();
    m_TogetherModeButton->m_Label.m_Text = "TOGETHER";
    m_TogetherModeButton->m_Size = size;
    m_TogetherModeButton->m_Label.m_Style.fontSize = fontSize;

    m_AiModeButton = new Button();
    m_AiModeButton->m_Label.m_Text = "AI";
    m_AiModeButton->m_Size = size;
    m_AiModeButton->m_Label.m_Style.fontSize = fontSize;

    m_MainLabel = new Label();
    m_MainLabel->m_Text = "Ping Pong";
    m_MainLabel->m_Style.fontSize = 40;

    m_BackButton = new Button();
    m_BackButton->m_Label.m_Text = "BACK";
    m_BackButton->m_Size = {100, 30};
    m_BackButton->m_Label.m_Style.fontSize = 15;

    m_BgPanel = new Panel();
    m_BgPanel->m_IsActive = false;
    m_BgPanel->m_Style.borderRadius = 0.1f;
    m_BgPanel->m_Style.outlineColor = {255, 255, 255, 100};
    m_BgPanel->setColor(shared::ColorRGBA::fromHex(0x1F1F1FCE));

    App()->UI()->add("Play", m_PlayButton);
    App()->UI()->add("Settings", m_SettingsButton);
    App()->UI()->add("Exit", m_ExitButton);
    App()->UI()->add("Back", m_BackButton);
    App()->UI()->add("TogetherMode", m_TogetherModeButton);
    App()->UI()->add("AiMode", m_AiModeButton);
    App()->UI()->add("MainLabel", m_MainLabel);
    App()->UI()->add("Panel", m_BgPanel);

    // OTHER
    App()->m_BackgroundColor = shared::ColorRGBA::fromHex(0x181818FF);
    m_ScreenTexture = App()->Graphics()->loadTexture("assets/textures/screen.png");
    m_CurrentWindow = WindowState::MAIN;
}

void MainMenuScene::update() {
    if (m_CurrentWindow == WindowState::MAIN) {
        if (m_PlayButton->m_LeftClickReleased) {
            m_CurrentWindow = WindowState::SELECT_MODE;
            m_MainLabel->m_Text = "Selected Mode";
            return;
        }
        if (m_SettingsButton->m_LeftClickReleased) {
            m_CurrentWindow = WindowState::SETTINGS;
            m_MainLabel->m_Text = "Settings";
            return;
        }
        if (m_ExitButton->m_LeftClickReleased) {
            App()->Window()->closeWindow();
            return;
        }
    }
    else if (m_CurrentWindow == WindowState::SELECT_MODE) {
        if (m_BackButton->m_LeftClickReleased) {
            m_CurrentWindow = WindowState::MAIN;
            m_MainLabel->m_Text = "Ping Pong";
            return;
        }
        if (m_TogetherModeButton->m_LeftClickReleased) {
            selectedPlayMode = PlayMode::TOGETHER;
            App()->setScene(new GameScene());
            return;
        }
        if (m_AiModeButton->m_LeftClickReleased) {
            selectedPlayMode = PlayMode::AI;
            App()->setScene(new GameScene());
            return;
        }
    }
    else if (m_CurrentWindow == WindowState::SETTINGS) {
        if (m_BackButton->m_LeftClickReleased) {
            m_CurrentWindow = WindowState::MAIN;
            m_MainLabel->m_Text = "Ping Pong";
            return;
        }
    }
}

void MainMenuScene::render() {
    shared::Vec2 s = App()->Window()->getScreenSize();

    float centerX = (s.x - m_PlayButton->m_Size.x) / 2.0f;
    float centerY = s.y / 2.0f;

    float newY = centerY - 80.0f;
    float buttonSpacing = 80.0f;

    renderBackground();

    if (m_CurrentWindow != WindowState::MAIN) {
        float panelW = 800.0f;
        float panelH = 550.0f;

        float panelX = (s.x - panelW) / 2.0f;
        float panelY = (s.y - panelH) / 2.0f;

        m_BgPanel->m_Size = { panelW, panelH };
        m_BgPanel->m_Position = { panelX, panelY };
        m_BgPanel->draw();

        m_BackButton->m_Position = { (s.x - m_BackButton->m_Size.x) / 2.0f, s.y - m_BackButton->m_Size.y - 20 };
        m_BackButton->draw();
    }

    if (m_CurrentWindow == WindowState::MAIN) {
        m_PlayButton->m_Position     = { centerX, newY };
        newY += buttonSpacing;

        m_SettingsButton->m_Position = { centerX, newY };
        newY += buttonSpacing;

        m_ExitButton->m_Position     = { centerX, newY };

        m_PlayButton->draw();
        m_SettingsButton->draw();
        m_ExitButton->draw();
    }
    else if (m_CurrentWindow == WindowState::SELECT_MODE) {
        m_TogetherModeButton->m_Position = { centerX, newY };
        newY += buttonSpacing;

        m_AiModeButton->m_Position = { centerX, newY };

        m_TogetherModeButton->draw();
        m_AiModeButton->draw();
    }
    else if (m_CurrentWindow == WindowState::SETTINGS) {
    }

    m_MainLabel->m_Position = {s.x / 2, 60};
    m_MainLabel->draw();
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
        15,
        {255, 255, 255, 100}
    );
}

void MainMenuScene::cleanup() {
    App()->UI()->remove("Play");
    App()->UI()->remove("Settings");
    App()->UI()->remove("Exit");
    App()->UI()->remove("Back");
    App()->UI()->remove("TogetherMode");
    App()->UI()->remove("AiMode");
    App()->UI()->remove("MainLabel");
    App()->UI()->remove("Panel");

    App()->Graphics()->unloadTexture(m_ScreenTexture);
}

// GAME
void GameScene::init() {
    m_Game = new GameController();
    m_Game->init(960, 540, 180, selectedPlayMode);

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
    if (m_Game) {
        m_Game->cleanup();
        delete m_Game;
        m_Game = nullptr;
    }
}