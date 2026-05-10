#pragma once

class GameController;

class PlayersController {
    GameController *m_Game;

public:
    void init(GameController *game);
    void update();
};
