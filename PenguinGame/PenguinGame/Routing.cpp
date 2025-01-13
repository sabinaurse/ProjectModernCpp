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
				return crow::response(400, "Player already in a game.");
			}

			auto dbPlayer = m_db.GetPlayerByName(playerName);
			auto player = std::make_unique<Player>(dbPlayer);

			//m_game.AddPlayerToQueue(player.get());
			m_game.AddPlayer(std::move(player));

			//m_game.TryStartMatch();

			return crow::response(200, "Player added to waiting queue: " + playerName);
		}
		catch (const std::exception& e) {
			return crow::response(500, "Error adding player to game: " + std::string(e.what()));
		}
			});

	CROW_ROUTE(m_app, "/getGameState").methods("GET"_method)([this]() {
		try {
			crow::json::wvalue gameState;

			// Obține lista de pinguini
			const auto& penguins = m_game.GetPenguins();
			std::cout << "Number of penguins in game: " << penguins.size() << std::endl;

			for (size_t i = 0; i < penguins.size(); ++i) {
				const auto* penguin = penguins[i].get();

				if (!penguin) {
					std::cout << "Penguin at index " << i << " is null." << std::endl;
					continue;
				}

				const auto* player = penguin->GetPlayer();
				if (!player) {
					std::cout << "Player for penguin at index " << i << " is null." << std::endl;
					continue;
				}

				std::cout << "Adding player to game state:"
					<< " Name: " << player->GetName()
					<< ", Position: (" << penguin->GetPosition().first
					<< ", " << penguin->GetPosition().second << ")" << std::endl;

				gameState["players"][i]["name"] = player->GetName();
				gameState["players"][i]["x"] = penguin->GetPosition().first;
				gameState["players"][i]["y"] = penguin->GetPosition().second;
			}

			return crow::response(200, gameState);
		}
		catch (const std::exception& e) {
			std::cerr << "Error retrieving game state: " << e.what() << std::endl;
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

			// e in weapon acum getsnowballs
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

			//penguin->UpgradeBulletSpeed();
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


	CROW_ROUTE(m_app, "/upgradePlayerWeapon").methods("POST"_method)
		([this](const crow::request& req) {
		try {
			auto body = crow::json::load(req.body);
			if (!body) {
				return crow::response(400, "Invalid JSON object.");
			}

			std::string playerName = body["playerName"].s();
			std::string upgradeType = body["upgradeType"].s();

			//m_game.UpgradePlayer(playerName, upgradeType);

			return crow::response(200, "Player weapon upgraded: " + upgradeType);
		}
		catch (const std::exception& e) {
			return crow::response(500, "Error upgrading player weapon: " + std::string(e.what()));
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
				//response["penguins"][i]["bulletSpeed"] = penguin->GetBulletSpeed();
				response["penguins"][i]["eliminations"] = penguin->GetEliminationOrder();
			}

			return crow::response(200, response);
		}
		catch (const std::exception& e) {
			return crow::response(500, "Error retrieving penguins: " + std::string(e.what()));
		}
		});




	m_app.port(port).bindaddr("0.0.0.0").multithreaded().run();
}
