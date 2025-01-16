#include "Routing.h"

Routing::Routing(GameManager& gameManager, game_database::PlayerDatabase& db)
	: m_gameManager{ gameManager }, m_db{ db }
{}


void Routing::Run(int port)
{
	CROW_ROUTE(m_app, "/")([]() {
		return "Server is running!";
		});

	CROW_ROUTE(m_app, "/addPlayer/<string>")([this](const std::string& name) {
		try {
			auto player = std::make_unique<Player>(name);

			game_database::GamePlayer playerRecord{ -1, name };
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


	CROW_ROUTE(m_app, "/updatePlayer").methods("POST"_method)
		([this](const crow::request& req) {
		try {
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
			if (!body || !body.has("name")) {
				return crow::response(400, "Invalid JSON object. Missing 'name'.");
			}

			std::string playerName = body["name"].s();

			// Obținem ID-ul jucătorului folosind metoda GetPlayerIdByName
			int playerId = m_gameManager.GetPlayerIdByName(playerName);
			if (playerId == -1) {
				return crow::response(400, "Player not found.");
			}

			// Obținem jocul corespunzător folosind metoda GetGameById
			Game* game = m_gameManager.GetGameById(playerId);
			if (!game) {
				return crow::response(400, "Player not in an active game.");
			}

			// Verificăm dacă jucătorul este deja în joc
			if (game->GetPlayerByName(playerName) != nullptr) {
				return crow::response(400, "Player already in the game.");
			}

			auto dbPlayer = m_db.GetPlayerByName(playerName);
			auto player = std::make_unique<Player>(dbPlayer);
			game->AddPlayer(std::move(player));


			return crow::response(200, "Player added to waiting queue: " + playerName);
		}
		catch (const std::exception& e) {
			return crow::response(500, "Error adding player to game: " + std::string(e.what()));
		}
			});

	CROW_ROUTE(m_app, "/getGameState").methods("POST"_method)
		([this](const crow::request& req) {
		try {
			auto body = crow::json::load(req.body);
			if (!body || !body.has("name")) {
				return crow::response(400, "Invalid JSON object. Missing 'name'.");
			}

			std::string playerName = body["name"].s();

			// Obținem ID-ul jucătorului folosind metoda GetPlayerIdByName
			int playerId = m_gameManager.GetPlayerIdByName(playerName);
			if (playerId == -1) {
				return crow::response(400, "Player not found.");
			}

			// Obținem jocul corespunzător folosind metoda GetGameById
			Game* game = m_gameManager.GetGameById(playerId);
			if (!game) {
				return crow::response(400, "Player not in an active game.");
			}

			crow::json::wvalue gameState;

			// Secțiunea pentru jucători
			const auto& penguins = game->GetPenguins();
			std::cout << "Number of penguins in game: " << penguins.size() << std::endl;

			for (size_t i = 0; i < penguins.size(); ++i) {
				const auto* penguin = penguins[i].get();

				if (!penguin->IsAlive()) {
					std::cout << "Penguin at index " << i << " is null." << std::endl;
					continue;
				}


				const auto* player = penguin->GetPlayer();
				if (!player) {
					std::cout << "Player for penguin at index " << i << " is null." << std::endl;
					continue;
				}

				// Adaugă informațiile despre jucători
				std::cout << "Adding player to game state:"
					<< " Name: " << player->GetName()
					<< ", Position: (" << penguin->GetPosition().first
					<< ", " << penguin->GetPosition().second << ")" << std::endl;

				gameState["players"][i]["name"] = player->GetName();
				gameState["players"][i]["x"] = penguin->GetPosition().first;
				gameState["players"][i]["y"] = penguin->GetPosition().second;
			}

			// Secțiunea pentru gloanțe
			size_t snowballIndex = 0; // Index pentru fiecare glonț
			for (const auto& penguin : penguins) {
				if (!penguin) continue;

				const auto& snowballs = penguin->GetWeapon().GetSnowballs();
				for (const auto& snowball : snowballs) {
					gameState["snowballs"][snowballIndex]["x"] = snowball.GetPosition().first;
					gameState["snowballs"][snowballIndex]["y"] = snowball.GetPosition().second;
					gameState["snowballs"][snowballIndex]["owner"] = penguin->GetPlayer()->GetName();
					gameState["snowballs"][snowballIndex]["active"] = snowball.IsActive();
					//gameState["snowballs"][snowballIndex]["direction"] = DirectionToString(snowball.GetDirection());
					++snowballIndex;
				}
			}

			if (game->MapUpdated()) {
				std::cout << "Map updated. Including updated map in response." << std::endl;
				gameState["board"] = game->GetBoardManager().SerializeBoard();
				game->ResetMapUpdateFlag();
			}
			else {
				std::cout << "No updates to map." << std::endl;
			}

			return crow::response(200, gameState);
		}
		catch (const std::exception& e) {
			std::cerr << "Error retrieving game state: " << e.what() << std::endl;
			return crow::response(500, "Error retrieving game state: " + std::string(e.what()));
		}
			});



	CROW_ROUTE(m_app, "/getMap").methods("POST"_method)
		([this](const crow::request& req) {
		try {
			auto body = crow::json::load(req.body);
			if (!body || !body.has("name")) {
				return crow::response(400, "Invalid JSON object. Missing 'name'.");
			}

			std::string playerName = body["name"].s();

			// Obținem ID-ul jucătorului folosind metoda GetPlayerIdByName
			int playerId = m_gameManager.GetPlayerIdByName(playerName);
			if (playerId == -1) {
				return crow::response(400, "Player not found.");
			}

			// Obținem jocul corespunzător folosind metoda GetGameById
			Game* game = m_gameManager.GetGameById(playerId);
			if (!game) {
				return crow::response(400, "Player not in an active game.");
			}

			crow::json::wvalue response;
			response["board"] = game->GetBoardManager().SerializeBoard();

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

	CROW_ROUTE(m_app, "/startGame").methods("POST"_method)([this](const crow::request& req) {
		try {
			auto body = crow::json::load(req.body);
			if (!body || !body.has("name")) {
				return crow::response(400, "Invalid JSON object. Missing 'name'.");
			}

			std::string playerName = body["name"].s();

			// Obținem ID-ul jucătorului folosind metoda GetPlayerIdByName
			int playerId = m_gameManager.GetPlayerIdByName(playerName);
			if (playerId == -1) {
				return crow::response(400, "Player not found.");
			}

			// Obținem jocul corespunzător folosind metoda GetGameById
			Game* game = m_gameManager.GetGameById(playerId);
			if (!game) {
				return crow::response(400, "Player not in an active game.");
			}
		
		
			const auto& players = game->GetPlayers();
			if (players.size() < 1) {
				return crow::response(400, "Not enough players to start the game. At least 2 players are required.");
			}

			game->StartGame();
			return crow::response(200, "Game started successfully.");
		}
		catch (const std::exception& e) {
			return crow::response(500, "Error starting game: " + std::string(e.what()));
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

			// Obținem ID-ul jucătorului folosind metoda GetPlayerIdByName
			int playerId = m_gameManager.GetPlayerIdByName(playerName);
			if (playerId == -1) {
				return crow::response(400, "Player not found.");
			}

			// Obținem jocul corespunzător folosind metoda GetGameById
			Game* game = m_gameManager.GetGameById(playerId);
			if (!game) {
				return crow::response(400, "Player not in an active game.");
			}

			Player* player = game->GetPlayerByName(playerName);
			if (!player) {
				return crow::response(404, "Player not found.");
			}

			Penguin* penguin = game->GetPenguinForPlayer(*player);
			if (!penguin) {
				return crow::response(404, "Penguin not found.");
			}

			penguin->Move(direction[0], game->GetBoard());
			return crow::response(200, "Move processed successfully");
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

			// Obținem ID-ul jucătorului folosind metoda GetPlayerIdByName
			int playerId = m_gameManager.GetPlayerIdByName(playerName);
			if (playerId == -1) {
				return crow::response(400, "Player not found.");
			}

			// Obținem jocul corespunzător folosind metoda GetGameById
			Game* game = m_gameManager.GetGameById(playerId);
			if (!game) {
				return crow::response(400, "Player not in an active game.");
			}

			Player* player = game->GetPlayerByName(playerName);
			if (!player) {
				CROW_LOG_ERROR << "Player not found: " << playerName;
				return crow::response(404, "Player not found.");
			}

			Penguin* penguin = game->GetPenguinForPlayer(*player);
			if (!penguin) {
				CROW_LOG_ERROR << "Penguin not found for player: " << playerName;
				return crow::response(404, "Penguin not found.");
			}

			penguin->Fire();

			const auto& snowballs = penguin->GetWeapon().GetSnowballs();
			if (snowballs.empty()) {
				CROW_LOG_ERROR << "No snowballs created for penguin of player: " << playerName;
				return crow::response(500, "No snowballs were created.");
			}

			const Snowball& latestSnowball = snowballs.back();
			CROW_LOG_INFO << "Snowball created at (" << latestSnowball.GetPosition().first << ", "
				<< latestSnowball.GetPosition().second << ") in direction "
				<< latestSnowball.GetDirection() << " with speed "
				<< latestSnowball.GetBulletSpeed();

			crow::json::wvalue response;
			response["startX"] = latestSnowball.GetPosition().first;
			response["startY"] = latestSnowball.GetPosition().second;
			response["direction"] = DirectionToString(latestSnowball.GetDirection());
			response["bulletSpeed"] = latestSnowball.GetBulletSpeed();

			return crow::response(200, response);
		}
		catch (const std::exception& e) {
			CROW_LOG_ERROR << "Error in /fire: " << e.what();
			return crow::response(500, "Error in /fire: " + std::string(e.what()));
		}
			});

	CROW_ROUTE(m_app, "/gameStatus").methods("POST"_method)
		([this](const crow::request& req) {
		try {
			auto body = crow::json::load(req.body);
			if (!body || !body.has("name")) {
				return crow::response(400, "Invalid JSON object. Missing 'name'.");
			}

			std::string playerName = body["name"].s();

			int gameId = m_gameManager.GetPlayerIdByName(playerName);
			if (gameId == -1) {
				return crow::response(404, "Player not found or not in a game.");
			}

			Game* game = m_gameManager.GetGameById(gameId);
			if (!game) {
				return crow::response(404, "Game not found.");
			}

			crow::json::wvalue response;
			response["gameId"] = game->GetGameId();
			response["status"] = game->IsGameRunning() ? "running" : "waiting";
			response["playerCount"] = game->GetPlayers().size();

			return crow::response(200, response);
		}
		catch (const std::exception& e) {
			return crow::response(500, "Error retrieving game status: " + std::string(e.what()));
		}
			});



	m_app.port(port).bindaddr("0.0.0.0").multithreaded().run();
}
