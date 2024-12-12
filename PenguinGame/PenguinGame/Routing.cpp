#include "Routing.h"

Routing::Routing(Game& game, game_database::PlayerDatabase& db) :m_game{ game }, m_db{ db }
{}

void Routing::Run(int port)
{
	CROW_ROUTE(m_app, "/")([]() {
		return "Server is running!";
		});

	CROW_ROUTE(m_app, "/addPlayer/<string>")([this](const std::string& name) {
		try {
			Player* newPlayer = new Player(name);

			game_database::GamePlayer playerRecord{ -1, name, 0, 0 };
			m_db.AddPlayer(playerRecord);

			return crow::response("Player added: " + name);
		}
		catch (const std::runtime_error& e) {
			return crow::response(400, "Error: " + std::string(e.what()));
		}
		catch (...) {
			return crow::response(500, "Unknown error occurred.");
		}
		});

	CROW_ROUTE(m_app, "/getPlayer/<string>")([this](const std::string& name) {
		try {
			game_database::GamePlayer dbPlayer = m_db.GetPlayerByName(name);
			Player* player = new Player(dbPlayer);

			crow::json::wvalue response;
			response["name"] = player->GetName();
			response["points"] = player->GetPoints();
			response["score"] = player->GetScore();

			delete player;

			return crow::response(response);

		}
		catch (const std::runtime_error& e) {
			return crow::response(404, std::string(e.what()));
		}
		catch (...) {
			return crow::response(500, "An unknown error occurred!");
		}
		});

	// ATENTIE : metodele Post si Delete de mai jos nu pot fi testate in browser -> avem nevoie de un proiect separat de Client pentru testare

	CROW_ROUTE(m_app, "/deletePlayer/<string>").methods("DELETE"_method)
		([this](const std::string& name) {
		try {

			game_database::GamePlayer dbplayer = m_db.GetPlayerByName(name);
			m_db.DeletePlayer(name);
			return crow::response(200, "Player deleted: " + name);
		}
		catch (const std::runtime_error& e) {
			// Captură pentru erorile de tip runtime
			return crow::response(400, "Error: " + std::string(e.what()));
		}
		catch (...) {
			// Captură pentru orice altă eroare neașteptată
			return crow::response(500, "Unknown error occurred.");
		}
			});

	CROW_ROUTE(m_app, "/updatePlayer").methods("POST"_method)
		([this](const crow::request& req) {
		try {
			// Parseaza request-ul JSON
			auto body = crow::json::load(req.body);

			if (!body) {
				return crow::response(400, "Invalid JSON object");
			}

			std::string name = body["name"].s();
			int new_score = body["score"].i();
			int new_points = body["points"].i();

			m_db.UpdatePlayerPoints(name, new_points);
			m_db.UpdatePlayerScore(name, new_score);

			return crow::response(200, "Player updated successfully");
		}
		catch (const std::exception& e) {
			return crow::response(400, "Error: " + std::string(e.what()));
		}
			});

	CROW_ROUTE(m_app, "/addPlayerToGame").methods("POST"_method)
		([this](const crow::request& req) {
		try {
			auto body = crow::json::load(req.body);
			if (!body) {
				return crow::response(400, "Invalid JSON object.");
			}

			std::string playerName = body["name"].s();

			if (m_game.GetPlayerByName(playerName) != nullptr) {
				return crow::response(400, "Player already in the game.");
			}

			Player* player = new Player(m_db.GetPlayerByName(playerName));
			m_game.AddPlayer(player);

			return crow::response(200, "Player added to the game successfully: " + playerName);
		}
		catch (const std::exception& e) {
			return crow::response(500, "Error adding player to game: " + std::string(e.what()));
		}
			});


	CROW_ROUTE(m_app, "/getMap")([this]() {
		try {
			crow::json::wvalue mapJson = m_game.GetBoard().SerializeBoard();
			return crow::response(200, mapJson);
		}
		catch (const std::exception& e) {
			return crow::response(500, "Error: " + std::string(e.what()));
		}
		});

	CROW_ROUTE(m_app, "/startGame").methods("POST"_method)([this]() {
		try {
			m_game.StartGame();
			return crow::response(200, "Game started successfully.");
		}
		catch (const std::exception& e) {
			return crow::response(500, "Error starting game: " + std::string(e.what()));
		}
		});

	CROW_ROUTE(m_app, "/resetGame").methods("POST"_method)([this]() {
		try {
			m_game.RestartGame();
			return crow::response(200, "Game reset successfully.");
		}
		catch (const std::exception& e) {
			return crow::response(500, "Error resetting game: " + std::string(e.what()));
		}
		});

	CROW_ROUTE(m_app, "/leaderboard") ([this]() {
		try {
			std::ostringstream leaderboard;
			leaderboard << "Leaderboard:\n";

			auto players = m_game.GetPlayers();
			std::sort(players.begin(), players.end(), [](Player* a, Player* b) {
				return a->GetPoints() > b->GetPoints();
				});

			for (const auto& player : players) {
				leaderboard << player->GetName() << " - " << player->GetPoints() << " points\n";
			}

			return crow::response(200, leaderboard.str());
		}
		catch (const std::exception& e) {
			return crow::response(500, "Error retrieving leaderboard: " + std::string(e.what()));
		}
		});

	CROW_ROUTE(m_app, "/movePlayer").methods("POST"_method)
		([this](const crow::request& req) {
		try {
			auto body = crow::json::load(req.body);
			if (!body) {
				return crow::response(400, "Invalid JSON object");
			}

			if (!body.has("direction")) {
				return crow::response(400, "Direction not provided");
			}

			std::string direction = body["direction"].s();
			if (direction.empty()) {
				return crow::response(400, "Direction cannot be empty");
			}

			char directionChar = direction[0];

			if (directionChar != 'W' && directionChar != 'A' && directionChar != 'S' && directionChar != 'D') {
				return crow::response(400, "Invalid direction. Use W, A, S, or D.");
			}

			std::string playerName = body["playerName"].s();
			Player* player = m_game.GetPlayerByName(playerName);

			if (!player) {
				return crow::response(404, "Player not found");
			}

			Penguin* penguin = m_game.GetPenguinForPlayer(*player);

			if (!penguin) {
				return crow::response(404, "Penguin not found");
			}

			penguin->Move(directionChar, m_game.GetBoard());
			return crow::response(200, "Player moved successfully");

		}
		catch (const std::exception& e) {
			return crow::response(500, "Error moving player: " + std::string(e.what()));
		}
			});

	CROW_ROUTE(m_app, "/fire").methods("POST"_method)
		([this](const crow::request& req) {
		try {
			auto body = crow::json::load(req.body);
			if (!body) {
				return crow::response(400, "Invalid JSON object");
			}

			std::string playerName = body["playerName"].s();
			Player* player = m_game.GetPlayerByName(playerName);
			if (!player) {
				return crow::response(404, "Player not found.");
			}

			Penguin* penguin = m_game.GetPenguinForPlayer(*player);
			if (!penguin) {
				return crow::response(404, "Penguin not found.");
			}

			penguin->Fire();
			return crow::response(200, "Snowball fired.");
		}
		catch (const std::exception& e) {
			return crow::response(500, "Error firing snowball: " + std::string(e.what()));
		}
			});

	CROW_ROUTE(m_app, "/checkCollisions").methods("POST"_method)
		([this]() {
		try {
			m_game.CheckForCollisions();
			return crow::response(200, "Collisions checked.");
		}
		catch (const std::exception& e) {
			return crow::response(500, "Error checking collisions: " + std::string(e.what()));
		}
			});

	CROW_ROUTE(m_app, "/upgradeBulletSpeed").methods("POST"_method)
		([this](const crow::request& req) {
		try {
			auto body = crow::json::load(req.body);
			if (!body) {
				return crow::response(400, "Invalid JSON object");
			}

			std::string playerName = body["playerName"].s();
			Player* player = m_game.GetPlayerByName(playerName);
			if (!player) {
				return crow::response(404, "Player not found.");
			}

			Penguin* penguin = m_game.GetPenguinForPlayer(*player);
			if (!penguin) {
				return crow::response(404, "Penguin not found.");
			}

			penguin->UpgradeBulletSpeed();
			return crow::response(200, "Bullet speed upgraded.");
		}
		catch (const std::exception& e) {
			return crow::response(500, "Error upgrading bullet speed: " + std::string(e.what()));
		}
			});


	CROW_ROUTE(m_app, "/stopGame").methods("POST"_method)
		([this]() {
		try {
			m_game.EndGame();
			return crow::response(200, "Game stopped successfully.");
		}
		catch (const std::exception& e) {
			return crow::response(500, "Error stopping game: " + std::string(e.what()));
		}
			});

	CROW_ROUTE(m_app, "/addMapElement").methods("POST"_method)
		([this](const crow::request& req) {
		try {
			auto body = crow::json::load(req.body);
			if (!body) {
				return crow::response(400, "Invalid JSON object.");
			}

			uint32_t x = body["x"].i();
			uint32_t y = body["y"].i();
			std::string elementType = body["elementType"].s();

			if (!m_game.GetBoard().IsWithinBounds(x, y)) {
				return crow::response(400, "Position out of bounds.");
			}

			Cell cellType;
			if (elementType == "bomb") {
				cellType = Cell::Hidden_Bomb;
			}
			else if (elementType == "destructible_wall") {
				cellType = Cell::Destructible_Wall;
			}
			else if (elementType == "indestructible_wall") {
				cellType = Cell::Indestructible_Wall;
			}
			else if (elementType == "empty") {
				cellType = Cell::Empty;
			}
			else {
				return crow::response(400, "Invalid element type.");
			}

			m_game.GetBoard().SetCell(x, y, cellType);
			return crow::response(200, "Element added successfully.");
		}
		catch (const std::exception& e) {
			return crow::response(500, "Error adding map element: " + std::string(e.what()));
		}
			});

	CROW_ROUTE(m_app, "/removeMapElement").methods("POST"_method)
		([this](const crow::request& req) {
		try {
			auto body = crow::json::load(req.body);
			if (!body) {
				return crow::response(400, "Invalid JSON object.");
			}

			uint32_t x = body["x"].i();
			uint32_t y = body["y"].i();

			if (!m_game.GetBoard().IsWithinBounds(x, y)) {
				return crow::response(400, "Position out of bounds.");
			}

			m_game.GetBoard().SetCell(x, y, Cell::Empty);
			return crow::response(200, "Element removed successfully.");
		}
		catch (const std::exception& e) {
			return crow::response(500, "Error removing map element: " + std::string(e.what()));
		}
			});







	m_app.port(18080).multithreaded().run();
}
