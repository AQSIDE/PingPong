#include "game.h"

#include "graphics.h"
#include "scene.h"
#include "window.h"
#include "../../../core/engine/include/ui.h"

shared::Vec2 getBallVelocity() {
    int randX = getRandomValue(0, 1);
    int randY = getRandomValue(0, 1);

    float velX = (randX == 1) ? -BALL_START_VELOCITY_X : BALL_START_VELOCITY_X;
    float velY = (randY == 1) ? -BALL_START_VELOCITY_Y : BALL_START_VELOCITY_Y;

    return shared::Vec2(velX, velY);
}

void checkCollisionPlayerWithBall(Player &p, Ball &b) {
    float pHalfW = p.m_Transform.m_Size.x / 2.0f;
    float pHalfH = p.m_Transform.m_Size.y / 2.0f;

    float ballRadius = b.m_Transform.m_Size.x / 2.0f;

    float diffX = b.m_Transform.m_Pos.x - p.m_Transform.m_Pos.x;
    float diffY = b.m_Transform.m_Pos.y - p.m_Transform.m_Pos.y;

    float closestX = std::clamp(diffX, -pHalfW, pHalfW);
    float closestY = std::clamp(diffY, -pHalfH, pHalfH);

    float normalX = diffX - closestX;
    float normalY = diffY - closestY;

    float distanceSq = (normalX * normalX) + (normalY * normalY);

    if (distanceSq < (ballRadius * ballRadius)) {
        float distance = sqrtf(distanceSq);

        if (distance == 0.0f) {
            normalX = 1.0f;
            normalY = 0.0f;
            distance = 1.0f;
        } else {
            normalX /= distance;
            normalY /= distance;
        }

        float penetration = ballRadius - distance;
        b.m_Transform.m_Pos.x += normalX * penetration;
        b.m_Transform.m_Pos.y += normalY * penetration;

        float dotProduct = (b.m_Transform.m_Vel.x * normalX) + (b.m_Transform.m_Vel.y * normalY);

        if (dotProduct < 0.0f) {
            b.m_Transform.m_Vel.x = b.m_Transform.m_Vel.x - 2.0f * dotProduct * normalX;
            b.m_Transform.m_Vel.y = b.m_Transform.m_Vel.y - 2.0f * dotProduct * normalY;

            float playerInfluenceY = p.m_Transform.m_Vel.y * 0.35f;
            b.m_Transform.m_Vel.y += playerInfluenceY;

            b.m_Transform.m_Vel = shared::Vec2::scale(b.m_Transform.m_Vel, 1.05f);
        }
    }
}

void checkCollisionBallWithBoard(shared::Vec2 fieldSize, Ball &b) {
    float halfBall = b.m_Transform.m_Size.y / 2.0f;

    if (b.m_Transform.m_Pos.y - halfBall < 0) {
        b.m_Transform.m_Pos.y = halfBall;
        b.m_Transform.m_Vel.y *= -1;
    }

    if (b.m_Transform.m_Pos.y + halfBall > fieldSize.y) {
        b.m_Transform.m_Pos.y = fieldSize.y - halfBall;
        b.m_Transform.m_Vel.y *= -1;
    }
}

void Ball::updateTrail() {
    m_BallTrail.m_TrailTimer += App()->getFrame().deltaTime;

    if (m_BallTrail.m_TrailTimer >= TRAIL_DELAY) {
        m_BallTrail.positions[m_BallTrail.head] = m_Transform.m_Pos;
        m_BallTrail.head = (m_BallTrail.head + 1) % TRAIL_SIZE;

        m_BallTrail.m_TrailTimer = 0.0f;
    }
}

void Ball::renderTrail() {
    auto *r = App()->Render();

    for (int i = 0; i < TRAIL_SIZE; i++) {
        int index = (m_BallTrail.head + i) % TRAIL_SIZE;

        float alpha = (float) (i + 1) / (float) TRAIL_SIZE;

        RenderContext trailCtx = m_RenderContext;
        trailCtx.color = shared::ColorRGBA::alpha(m_RenderContext.color, alpha * 0.5f);

        Transform2D tempTr = m_Transform;
        tempTr.m_Pos = m_BallTrail.positions[index];
        tempTr.m_Size = shared::Vec2::scale(m_Transform.m_Size, alpha);

        r->drawItem(&tempTr, &trailCtx);
    }
}

void GameController::init(int fieldW, int fieldH, float gameTime, PlayMode playMode) {
    auto sc = App()->Window()->getScreenSize();
    auto *graphics = App()->Graphics();

    // INIT
    m_FieldWidth = fieldW;
    m_FieldHeight = fieldH;

    m_PlayersController.init(this);
    m_Player1Score = 0;
    m_Player2Score = 0;
    m_Timer = gameTime;
    m_PlayMode = playMode;
    m_CooldownTimer = COOLDOWN_TIME;
    m_GameState = GameState::IDLE;
    m_BoardTheme = COLORS::getBoardTheme();

    // SPAWN
    float centerX = (float) m_FieldWidth / 2;
    float centerY = (float) m_FieldHeight / 2;
    float padding = 30;

    m_Player1.m_Transform.m_Size = {30, 130};
    m_Player2.m_Transform.m_Size = {30, 130};
    m_Ball.m_Transform.m_Size = {30, 30};
    m_Ball.m_Transform.m_Vel = getBallVelocity();

    //p1
    m_Player1.m_Transform.m_Rot = 180;
    m_Player1.m_Transform.m_Pos.x = padding;
    m_Player1.m_Transform.m_Pos.y = centerY;

    //p2
    m_Player2.m_Transform.m_Pos.x = (float) m_FieldWidth - padding;
    m_Player2.m_Transform.m_Pos.y = centerY;

    // ball
    m_Ball.m_Transform.m_Pos.x = centerX;
    m_Ball.m_Transform.m_Pos.y = centerY;

    // TEXTURE
    m_PlayerTexture = graphics->loadTexture("assets/textures/player.png");
    m_BallTexture = graphics->loadTexture("assets/textures/ball.png");
    m_scoreBarTexture = graphics->loadTexture("assets/textures/score_bar.png");
    m_BloomShader = graphics->loadShader("assets/shaders/bloom.fs", "");
    m_BloomTarget = graphics->createRenderTexture((int) sc.x, (int) sc.y);

    m_Player1.m_RenderContext.texture = &m_PlayerTexture;
    m_Player2.m_RenderContext.texture = &m_PlayerTexture;
    m_Ball.m_RenderContext.texture = &m_BallTexture;

    m_Player1.m_RenderContext.color = {0, 228, 48, 255};
    m_Player2.m_RenderContext.color = {0, 121, 241, 255};
    m_Ball.m_RenderContext.color = COLOR_WHITE;

    auto* timerLabel = new Label();
    timerLabel->m_Style.fontSize = 40;

    auto* score1Label = new Label();
    score1Label->m_Style.fontSize = 30;

    auto* score2Label = new Label();
    score2Label->m_Style.fontSize = 30;

    // UI
    App()->UI()->add("ScoreP1", score1Label);
    App()->UI()->add("ScoreP2", score2Label);
    App()->UI()->add("Timer", timerLabel);

    auto endBtn = new Button();
    endBtn->m_Size = {300, 50};
    endBtn->m_Label.m_Text = "Back to menu";
    endBtn->m_Label.m_Style.fontSize = 20;

    auto* text1 = new Label("GAME FINISHED");
    text1->m_Style.fontSize = 30;

    auto* text2 = new Label("SCORE");
    text2->m_Style.fontSize = 30;

    App()->UI()->add("Text1", text1);
    App()->UI()->add("Text2", text2);
    App()->UI()->add("EndBtn", endBtn);
}

void GameController::cleanup() {
    App()->UI()->remove("ScoreP1");
    App()->UI()->remove("ScoreP2");
    App()->UI()->remove("Timer");
    App()->UI()->remove("Text1");
    App()->UI()->remove("EndBtn");
    App()->UI()->remove("Text2");

    App()->Graphics()->unloadTexture(m_PlayerTexture);
    App()->Graphics()->unloadTexture(m_BallTexture);
    App()->Graphics()->unloadTexture(m_scoreBarTexture);
    App()->Graphics()->unloadShader(m_BloomShader);
    App()->Graphics()->unloadRenderTexture(m_BloomTarget);
}

void GameController::update() {
    if (m_GameState == GameState::FINISHED) {
        auto *endBtn = App()->UI()->get<Button>("EndBtn");
        if (endBtn->m_LeftClickPressed) {
            App()->setScene(new MainMenuScene);
            return;
        }

        return;
    }
    float dt = App()->getFrame().deltaTime;

    if (m_GameState == GameState::IDLE) {
        m_CooldownTimer -= dt;
        if (m_CooldownTimer <= 0) {
            m_CooldownTimer = 0;
            m_GameState = GameState::ACTIVE;
        }

        return;
    }

    if (m_GameState != GameState::ACTIVE) return;
    m_Timer -= dt;
    if (m_Timer < 0) {
        m_Timer = 0;
        m_GameState = GameState::FINISHED;
        return;
    }

    m_PlayersController.update();
    m_Ball.m_Transform.m_Pos.x += m_Ball.m_Transform.m_Vel.x * dt;
    m_Ball.m_Transform.m_Pos.y += m_Ball.m_Transform.m_Vel.y * dt;

    checkCollisionPlayerWithBall(m_Player1, m_Ball);
    checkCollisionPlayerWithBall(m_Player2, m_Ball);
    checkCollisionBallWithBoard(getFieldSize(), m_Ball);

    m_Ball.updateTrail();
    m_Ball.m_Transform.rotate(200 * dt);

    if (m_Ball.m_Transform.m_Pos.x < 0) {
        resetBall();
        m_Player2Score++;
    }

    if (m_Ball.m_Transform.m_Pos.x > m_FieldWidth) {
        resetBall();
        m_Player1Score++;
    }
}

void GameController::render() {
    auto *graphics = App()->Graphics();

    graphics->beginRenderTexture(m_BloomTarget);
    graphics->clearColor(COLOR_BLANK);
    graphics->beginCamera(App()->Camera()->getData());
    drawBoard();
    renderPlayersAndBall();
    graphics->endCamera();
    graphics->endRenderTexture();

    graphics->beginShader(m_BloomShader);
    float w = (float) m_BloomTarget.texture.width;
    float h = (float) m_BloomTarget.texture.height;

    graphics->drawTexture(
        m_BloomTarget.texture,
        {0, 0, w, -h},
        {0, 0, w, h},
        {0, 0});

    graphics->endShader();

    if (m_GameState == GameState::ACTIVE) {
        drawTimer();
        drawScore();
    } else if (m_GameState == GameState::FINISHED) {
        renderEndScreen();
    }
    else if (m_GameState == GameState::IDLE) {
        renderCooldownScreen();
    }
}

void GameController::renderPlayersAndBall() {
    auto renderer = App()->Render();

    if (m_GameState == GameState::ACTIVE)
        m_Ball.renderTrail();

    renderer->drawItem(&m_Ball.m_Transform, &m_Ball.m_RenderContext);
    renderer->drawItem(&m_Player1.m_Transform, &m_Player1.m_RenderContext);
    renderer->drawItem(&m_Player2.m_Transform, &m_Player2.m_RenderContext);
}

void GameController::drawScore() {
    // DRAW SCORE BAR
    float width = 500.0f;
    float height = 60.0f;
    shared::Rect source = {0.0f, 0.0f, (float) m_scoreBarTexture.width, (float) m_scoreBarTexture.height};
    shared::Vec2 origin = {width / 2, height / 2};

    shared::Rect destLeft = {width / 2, height / 2 + 10, width, height};
    App()->Graphics()->drawTexture(m_scoreBarTexture, source, destLeft, origin, 0, m_BoardTheme.mainColor);

    shared::Rect destRight = {App()->Window()->getScreenSize().x - width / 2, height / 2 + 10, width, height};
    App()->Graphics()->drawTexture(m_scoreBarTexture, source, destRight, origin, 180, m_BoardTheme.mainColor);

    //DRAW SCORE
    float margin = 40;
    float posY = 40;

    float posXScore1 = margin;
    float posXScore2 = App()->Window()->getScreenSize().x - margin;

    auto *score1 = App()->UI()->get<Label>("ScoreP1");
    auto *score2 = App()->UI()->get<Label>("ScoreP2");

    score1->m_Position = {posXScore1, posY};
    score2->m_Position = {posXScore2, posY};

    score1->m_Text = std::to_string(m_Player1Score);
    score2->m_Text = std::to_string(m_Player2Score);

    score1->draw();
    score2->draw();
}

void GameController::drawTimer() {
    float posX = App()->Window()->getScreenSize().x / 2;
    float posY = 40;

    auto *timer = App()->UI()->get<Label>("Timer");
    timer->m_Position = {posX, posY};
    timer->m_Text = std::to_string((int) m_Timer);
    timer->draw();
}

void GameController::drawBoard() {
    App()->Graphics()->drawRect({0, 0, (float) m_FieldWidth, (float) m_FieldHeight}, m_BoardTheme.boardColor);

    int cellSize = 40;
    shared::ColorRGBA gridColor = m_BoardTheme.mainColor;
    gridColor.a = 30;

    for (int x = 0; x <= m_FieldWidth; x += cellSize) {
        App()->Graphics()->drawLine({(float) x, 0}, {(float) x, (float) m_FieldHeight}, 1, gridColor);
    }
    for (int y = 0; y <= m_FieldHeight; y += cellSize) {
        App()->Graphics()->drawLine({0, (float) y}, {(float) m_FieldWidth, (float) y}, 1, gridColor);
    }

    float centerX = m_FieldWidth / 2.0f;
    App()->Graphics()->drawLine({centerX, 0}, {centerX, (float) m_FieldHeight}, 1, m_BoardTheme.mainColor);
    App()->Graphics()->drawRectLines({0, 0, (float) m_FieldWidth, (float) m_FieldHeight}, 1, m_BoardTheme.mainColor);
}

void GameController::renderEndScreen() {
    auto sc = App()->Window()->getScreenSize();
    App()->Graphics()->drawRect({0, 0, sc.x, sc.y}, {0, 0, 0, 180});

    auto *endTxt = App()->UI()->get<Label>("Text1");
    endTxt->m_Text = "GAME FINISHED";
    endTxt->m_Style.fontSize = 50;
    endTxt->m_Position = {sc.x / 2.0f, sc.y * 0.3f};
    endTxt->draw();

    auto *endTextScore = App()->UI()->get<Label>("Text2");
    endTextScore->m_Text = std::to_string(m_Player1Score) + " - " + std::to_string(m_Player2Score);
    endTextScore->m_Position = {sc.x / 2.0f, sc.y * 0.4f};
    endTextScore->draw();

    auto *endBtn = App()->UI()->get<Button>("EndBtn");
    endBtn->m_Position = {sc.x / 2.0f - endBtn->m_Size.x / 2.0f, sc.y * 0.5f};
    endBtn->draw();
}

void GameController::renderCooldownScreen() {
    auto sc = App()->Window()->getScreenSize();
    App()->Graphics()->drawRect({0, 0, sc.x, sc.y}, {0, 0, 0, 180});

    auto *text = App()->UI()->get<Label>("Text1");
    text->m_Position = {sc.x / 2.0f, sc.y / 2.0f};
    text->m_Style.fontSize = 60;

    std::string txt;
    if (m_CooldownTimer > 1)
        txt = std::to_string((int)m_CooldownTimer);
    else
        txt = "GO!";

    text->m_Text = txt;
    text->draw();
}

shared::Vec2 GameController::getFieldCenter() {
    return {(float) m_FieldWidth / 2, (float) m_FieldHeight / 2};
}

shared::Vec2 GameController::getFieldSize() {
    return {(float) m_FieldWidth, (float) m_FieldHeight};
}

Player &GameController::getP1() {
    return m_Player1;
}

Player &GameController::getP2() {
    return m_Player2;
}

const Ball &GameController::getBall() const {
    return m_Ball;
}

void GameController::updateState(GameState newState) {
    m_GameState = newState;
}

GameState GameController::getState() const {
    return m_GameState;
}

PlayMode GameController::getPlayMode() const {
    return m_PlayMode;
}

void GameController::resetBall() {
    shared::Vec2 center = getFieldCenter();

    m_Ball.m_Transform.m_Pos.x = center.x;
    m_Ball.m_Transform.m_Pos.y = center.y;
    m_Ball.m_Transform.m_Vel = getBallVelocity();
}