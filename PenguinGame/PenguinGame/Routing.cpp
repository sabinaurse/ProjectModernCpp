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
			auto player = std::make_unique<Player>(name);

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
			auto player = std::make_unique<Player>(dbPlayer);

			crow::json::wvalue response;
			response["name"] = player->GetName();
			response["points"] = player->GetPoints();
			response["score"] = player->GetScore();
			response["bullet_speed_level"] = player->GetBulletSpeedLevel();
			response["cooldown_level"] = player->GetCooldownLevel();

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

	/*CROW_ROUTE(m_app, "/addPlayerToGame").methods("POST"_method)
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
			auto player = std::make_unique<Player>(m_db.GetPlayerByName(playerName));
			m_game.AddPlayer(std::move(player));

			return crow::response(200, "Player added to the game successfully: " + playerName);
		}
		catch (const std::exception& e) {
			return crow::response(500, "Error adding player to game: " + std::string(e.what()));
		}
			});*/

	CROW_ROUTE(m_app, "/addPlayerToGame").methods("POST"_method)
		([this](const crow::request& req) {
		try {
			auto body = crow::json::load(req.body);
			if (!body) {
				return crow::response(400, "Invalid JSON object.");
			}

			std::string playerName = body["name"].s();

			if (m_game.GetPlayerByName(playerName) != nullptr) {
				return crow::response(400, "Player already in a game.");
			}

			auto dbPlayer = m_db.GetPlayerByName(playerName);
			auto player = std::make_unique<Player>(dbPlayer);

			m_game.AddPlayerToQueue(player.get());
			m_game.AddPlayer(std::move(player));

			m_game.TryStartMatch();

			return crow::response(200, "Player added to waiting queue: " + playerName);
		}
		catch (const std::exception& e) {
			return crow::response(500, "Error adding player to game: " + std::string(e.what()));
		}
			});

	CROW_ROUTE(m_app, "/getGameState").methods("GET"_method)([this]() {
		try {
			crow::json::wvalue gameState;

			const auto& penguins = m_game.GetPenguins();
			for (size_t i = 0; i < penguins.size(); ++i) {
				const auto* penguin = penguins[i].get();
				gameState["players"][i]["name"] = penguin->GetPlayer()->GetName();
				gameState["players"][i]["x"] = penguin->GetPosition().first;
				gameState["players"][i]["y"] = penguin->GetPosition().second;
			}

			return crow::response(200, gameState);
		}
		catch (const std::exception& e) {
			return crow::response(500, "Error retrieving game state: " + std::string(e.what()));
		}
		});

	CROW_ROUTE(m_app, "/getMap").methods("GET"_method)([this]() {
		try {
			crow::json::wvalue response;
			response["board"] = m_game.GetBoardManager().SerializeBoard();

			response["cellTypes"] = {
				{"0", "Empty"},
				{"1", "Destructible Wall"},
				{"2", "Indestructible Wall"},
				{"3", "Bomb"}
			};

			//std::string serializedResponse = response.dump();
			//std::cout << "Response JSON: " << serializedResponse << std::endl;

			return crow::response(200, response);
		}
		catch (const std::exception& e) {
			return crow::response(500, "Error retrieving map: " + std::string(e.what()));
		}
		});

	CROW_ROUTE(m_app, "/startGame").methods("POST"_method)([this]() {
		try {
			const auto& players = m_game.GetPlayers();
			if (players.size() < 1) {
				return crow::response(400, "Not enough players to start the game. At least 2 players are required.");
			}

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

			auto& players = m_game.GetPlayers();
			std::sort(players.begin(), players.end(), [](const std::unique_ptr<Player>& a, const std::unique_ptr<Player>& b) {
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
			if (!body || !body.has("direction") || !body.has("playerName")) {
				return crow::response(400, "Invalid request body");
			}

			std::string direction = body["direction"].s();
			std::string playerName = body["playerName"].s();

			Player* player = m_game.GetPlayerByName(playerName);
			if (!player) {
				return crow::response(404, "Player not found.");
			}

			Penguin* penguin = m_game.GetPenguinForPlayer(*player);
			if (!penguin) {
				return crow::response(404, "Penguin not found.");
			}

			penguin->Move(direction[0], m_game.GetBoard());

			crow::json::wvalue response;
			response["x"] = penguin->GetPosition().first;
			response["y"] = penguin->GetPosition().second;

			return crow::response(200, response);
		}
		catch (const std::exception& e) {
			return crow::response(500, "Error moving player: " + std::string(e.what()));
		}
			});
	CROW_ROUTE(m_app, "/fire").methods("POST"_method)
		([this](const crow::request& req) {
		try {
			auto body = crow::json::load(req.body);
			CROW_LOG_INFO << "Request body: " << req.body;

			if (!body || !body.has("playerName")) {
				return crow::response(400, "Invalid request: Missing playerName.");
			}

			std::string playerName = body["playerName"].s();

			Player* player = m_game.GetPlayerByName(playerName);
			if (!player) {
				CROW_LOG_ERROR << "Player not found: " << playerName;
				return crow::response(404, "Player not found.");
			}

			Penguin* penguin = m_game.GetPenguinForPlayer(*player);
			if (!penguin) {
				CROW_LOG_ERROR << "Penguin not found for player: " << playerName;
				return crow::response(404, "Penguin not found.");
			}

			penguin->Fire();

			const auto& snowballs = penguin->GetSnowballs();
			if (snowballs.empty()) {
				CROW_LOG_ERROR << "No snowballs created for penguin of player: " << playerName;
				return crow::response(500, "No snowballs were created.");
			}

			const Snowball& latestSnowball = snowballs.back();
			CROW_LOG_INFO << "Snowball created at (" << latestSnowball.GetPosition().first << ", "
				<< latestSnowball.GetPosition().second << ") in direction "
				<< latestSnowball.GetDirection() << " with speed "
				<< latestSnowball.GetSpeed();

			crow::json::wvalue response;
			response["startX"] = latestSnowball.GetPosition().first;
			response["startY"] = latestSnowball.GetPosition().second;
			response["direction"] = latestSnowball.GetDirection();
			response["bulletSpeed"] = latestSnowball.GetSpeed();

			return crow::response(200, response);
		}
		catch (const std::exception& e) {
			CROW_LOG_ERROR << "Error in /fire: " << e.what();
			return crow::response(500, "Error in /fire: " + std::string(e.what()));
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

			int32_t x = body["x"].i();
			int32_t y = body["y"].i();
			std::string elementType = body["elementType"].s();

			if (!m_game.GetBoardManager().IsWithinBounds(x, y)) {
				return crow::response(400, "Position out of bounds.");
			}

			int cellType;
			if (elementType == "bomb") {
				cellType = 3;
			}
			else if (elementType == "destructible_wall") {
				cellType = 1;
			}
			else if (elementType == "indestructible_wall") {
				cellType = 2;
			}
			else if (elementType == "empty") {
				cellType = 0;
			}
			else {
				return crow::response(400, "Invalid element type.");
			}

			m_game.GetBoardManager().DestroyCell(x, y);
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

			int32_t x = body["x"].i();
			int32_t y = body["y"].i();

			if (!m_game.GetBoardManager().IsWithinBounds(x, y)) {
				return crow::response(400, "Position out of bounds.");
			}

			m_game.GetBoardManager().SetCell(x, y, 0);
			return crow::response(200, "Element removed successfully.");
		}
		catch (const std::exception& e) {
			return crow::response(500, "Error removing map element: " + std::string(e.what()));
		}
			});

	CROW_ROUTE(m_app, "/resetPlayerLives").methods("POST"_method)
		([this]() {
		try {
			const auto& penguins = m_game.GetPenguins();
			for (const auto& penguin : penguins) {
				if (penguin) {
					penguin->ResetState();
				}
			}

			return crow::response(200, "All player lives have been reset.");
		}
		catch (const std::exception& e) {
			return crow::response(500, "Error resetting player lives: " + std::string(e.what()));
		}
			});

	CROW_ROUTE(m_app, "/upgradePlayerWeapon").methods("POST"_method)
		([this](const crow::request& req) {
		try {
			auto body = crow::json::load(req.body);
			if (!body) {
				return crow::response(400, "Invalid JSON object.");
			}

			std::string playerName = body["playerName"].s();
			std::string upgradeType = body["upgradeType"].s();

			m_game.UpgradePlayer(playerName, upgradeType);

			return crow::response(200, "Player weapon upgraded: " + upgradeType);
		}
		catch (const std::exception& e) {
			return crow::response(500, "Error upgrading player weapon: " + std::string(e.what()));
		}
			});

	CROW_ROUTE(m_app, "/addBonusToMap").methods("POST"_method)
		([this](const crow::request& req) {
		try {
			auto body = crow::json::load(req.body);
			if (!body) {
				return crow::response(400, "Invalid JSON object.");
			}

			uint32_t x = body["x"].i();
			uint32_t y = body["y"].i();
			std::string bonusType = body["bonusType"].s();

			if (!m_game.GetBoardManager().IsWithinBounds(x, y)) {
				return crow::response(400, "Cell out of bounds.");
			}

			// Adăugăm bonusul specificat.
			// Logica pentru stocarea bonusurilor ar putea necesita o extensie a clasei `GameBoard`.

			return crow::response(200, "Bonus added to map: " + bonusType);
		}
		catch (const std::exception& e) {
			return crow::response(500, "Error adding bonus: " + std::string(e.what()));
		}
			});

	CROW_ROUTE(m_app, "/resetPenguinPositions").methods("POST"_method)([this]() {
		try {
			const auto& penguins = m_game.GetPenguins();
			for (const auto& penguin : penguins) {
				penguin->ResetState();
			}

			return crow::response(200, "All penguin positions reset to initial state.");
		}
		catch (const std::exception& e) {
			return crow::response(500, "Error resetting penguin positions: " + std::string(e.what()));
		}
		});

	CROW_ROUTE(m_app, "/getCellDetails/<int>/<int>").methods("GET"_method)
		([this](int x, int y) {
		try {
			if (!m_game.GetBoardManager().IsWithinBounds(x, y)) {
				return crow::response(400, "Position out of bounds.");
			}

			int cellType = m_game.GetBoardManager().GetCell(x, y);

			crow::json::wvalue response;
			response["x"] = x;
			response["y"] = y;
			response["cellType"] = cellType;

			return crow::response(200, response);
		}
		catch (const std::exception& e) {
			return crow::response(500, "Error retrieving cell details: " + std::string(e.what()));
		}
			});

	CROW_ROUTE(m_app, "/changeMapElement").methods("POST"_method)
		([this](const crow::request& req) {
		try {
			auto body = crow::json::load(req.body);
			if (!body) {
				return crow::response(400, "Invalid JSON object.");
			}

			int x = body["x"].i();
			int y = body["y"].i();
			int newCellType = body["newCellType"].i();

			if (!m_game.GetBoardManager().IsWithinBounds(x, y)) {
				return crow::response(400, "Position out of bounds.");
			}

			m_game.GetBoardManager().SetCell(x, y, newCellType);

			return crow::response(200, "Cell updated successfully.");
		}
		catch (const std::exception& e) {
			return crow::response(500, "Error updating map element: " + std::string(e.what()));
		}
			});

	CROW_ROUTE(m_app, "/getStartingPositions").methods("GET"_method)([this]() {
		try {
			crow::json::wvalue response;
			const auto& startingPositions = m_game.GetBoardManager().GetStartingPositions();

			for (size_t i = 0; i < startingPositions.size(); ++i) {
				response["positions"][i]["x"] = startingPositions[i].first;
				response["positions"][i]["y"] = startingPositions[i].second;
			}

			return crow::response(200, response);
		}
		catch (const std::exception& e) {
			return crow::response(500, "Error retrieving starting positions: " + std::string(e.what()));
		}
		});

	CROW_ROUTE(m_app, "/initializePenguins").methods("POST"_method)([this]() {
		try {
			const auto& startingPositions = m_game.GetBoardManager().GetStartingPositions();
			const auto& players = m_game.GetPlayers();

			if (players.size() != startingPositions.size()) {
				return crow::response(400, "Mismatch between players and starting positions.");
			}

			for (size_t i = 0; i < players.size(); ++i) {
				auto* player = players[i].get();
				Position position = { startingPositions[i].first, startingPositions[i].second };
				m_game.GetBoardManager().AddPenguin(std::make_shared<Penguin>(player, position, 500));
			}

			return crow::response(200, "Penguins initialized on starting positions.");
		}
		catch (const std::exception& e) {
			return crow::response(500, "Error initializing penguins: " + std::string(e.what()));
		}
		});

	CROW_ROUTE(m_app, "/getPenguins").methods("GET"_method)([this](const crow::request& req) {
		try {
			crow::json::wvalue response;
			const auto& penguins = m_game.GetPenguins();

			for (size_t i = 0; i < penguins.size(); ++i) {
				const auto* penguin = penguins[i].get();
				response["penguins"][i]["name"] = penguin->GetPlayer()->GetName();
				response["penguins"][i]["x"] = penguin->GetPosition().first;
				response["penguins"][i]["y"] = penguin->GetPosition().second;
				response["penguins"][i]["isAlive"] = penguin->IsAlive();
				response["penguins"][i]["bulletSpeed"] = penguin->GetBulletSpeed();
				response["penguins"][i]["eliminations"] = penguin->GetEliminationOrder();
			}

			return crow::response(200, response);
		}
		catch (const std::exception& e) {
			return crow::response(500, "Error retrieving penguins: " + std::string(e.what()));
		}
		});




	m_app.port(18080).multithreaded().run();
}
