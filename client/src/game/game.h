#pragma once
#include "entity.h"
#include "players.h"
#include "style.h"

constexpr float BALL_START_VELOCITY_X = 300;
constexpr float BALL_START_VELOCITY_Y = 300;
constexpr float GAME_TIME = 60;
constexpr float COOLDOWN_TIME = 5;

#define TRAIL_SIZE 10
#define TRAIL_DELAY 0.02f

class Player : public IEntity {
public:
    Player() : IEntity() {
    }
};

struct BallTrail {
    float m_TrailTimer = 0.0f;
    shared::Vec2 positions[TRAIL_SIZE];
    int head = 0;
};

class Ball : public IEntity {
    BallTrail m_BallTrail;
public:
    Ball() : IEntity() {}

    void renderTrail();
    void updateTrail();
};

enum class GameState {
    IDLE,
    ACTIVE,
    FINISHED,
};

class GameController {
    Player m_Player1;
    Player m_Player2;
    Ball m_Ball;
    GameState m_GameState;

    PlayersController m_PlayersController;

    TextureHandle m_PlayerTexture;
    TextureHandle m_BallTexture;
    TextureHandle m_scoreBarTexture;
    RenderTextureHandle m_BloomTarget;
    ShaderHandle m_BloomShader;

    int m_FieldWidth;
    int m_FieldHeight;
    float m_Timer;
    float m_CooldownTimer;

    int m_Player1Score;
    int m_Player2Score;

    COLORS::BoardTheme m_BoardTheme;

    void drawTimer();
    void drawScore();
    void drawBoard();
    void renderPlayersAndBall();
    void renderEndScreen();
    void renderCooldownScreen();
public:
    void init(int fieldW, int fieldH);
    void cleanup();
    void render();
    void update();

    Player& getP1();
    Player& getP2();
    shared::Vec2 getFieldCenter();
    shared::Vec2 getFieldSize();
    void resetBall();
    void updateState(GameState newState);
    GameState getState() const;
};
