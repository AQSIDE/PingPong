#pragma once
#include "engine.h"
#include "ui.h"
#include "game.h"

class MainMenuScene : public IScene {
    Button* m_PlayButton = nullptr;
    Label* m_NameGameLabel = nullptr;
    TextureHandle m_ScreenTexture;

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