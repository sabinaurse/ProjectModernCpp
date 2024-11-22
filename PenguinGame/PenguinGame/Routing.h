#pragma once
#include <crow.h>
#include "Game.h"
#include "GameDatabase.h"

class Routing
{
public:
	Routing(Game& game, game_database::PlayerDatabase& db);
	void Run(int port = 18080);
private:
	crow::SimpleApp m_app;
	Game& m_game;
	game_database::PlayerDatabase& m_db;
};



