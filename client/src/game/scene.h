#pragma once
#include "engine.h"
#include "ui.h"
#include "game.h"

class MainMenuScene : public IScene {
    enum class WindowState {
        MAIN,
        SETTINGS,
        SELECT_MODE,
    };

    WindowState m_CurrentWindow = WindowState::MAIN;
    TextureHandle m_ScreenTexture;

    Button* m_PlayButton = nullptr;
    Button* m_SettingsButton = nullptr;
    Button* m_BackButton = nullptr;
    Button* m_ExitButton = nullptr;
    Button* m_TogetherModeButton = nullptr;
    Button* m_AiModeButton = nullptr;
    Label* m_MainLabel = nullptr;
    Panel* m_BgPanel = nullptr;

    void renderBackground();
public:
    void init() override;
    void update() override;
    void render() override;
    void cleanup() override;
};

class GameScene : public IScene {
    GameController* m_Game = nullptr;
public:
    void init() override;
    void update() override;
    void render() override;
    void cleanup() override;
};