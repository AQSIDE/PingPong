#pragma once

class GameController;

class PlayersController {
    GameController *m_Game = nullptr;

    void processInput(float dt, float speed, float& v1, float& v2);
public:
    void init(GameController *game);
    void update();
};
