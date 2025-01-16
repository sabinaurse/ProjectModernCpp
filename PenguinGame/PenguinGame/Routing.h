#pragma once
#include <crow.h>
#include "GameManager.h"
#include "GameDatabase.h"

class Routing
{
public:
    Routing(GameManager& gameManager, game_database::PlayerDatabase& db);
    void Run(int port = 18080);
private:
    crow::SimpleApp m_app;
    GameManager& m_gameManager;
    game_database::PlayerDatabase& m_db;
};



