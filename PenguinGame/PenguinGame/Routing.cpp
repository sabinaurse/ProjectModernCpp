#include "Routing.h"

Routing::Routing(Game& game, game_database::PlayerDatabase& db) :m_game{ game }, m_db{ db }
{}

void Routing::Run(int port)
{
	CROW_ROUTE(m_app, "/")([]() {
		return "Server is running!";
		});

	// Ruta pentru adaugarea unui jucator
	CROW_ROUTE(m_app, "/addPlayer/<string>")([this](const std::string& name) {
		try {
			Player* newPlayer = new Player(name);
			m_game.AddPlayer(newPlayer); // <- adaugarea in baza de date si adaugarea in joc a player-ului ar trebui sa fie in aceasi ruta ?

			// Adaugam jucatorul si in baza de date
			game_database::GamePlayer playerRecord{ -1, name, 0, 0 };
			m_db.AddPlayer(playerRecord);

			return crow::response("Player added: " + name);
		}
		catch (const std::runtime_error& e) {
			// Prinde erorile de runtime, cum ar fi conflicte de date
			return crow::response(400, "Error: " + std::string(e.what()));
		}
		catch (...) {
			// Erori neașteptate
			return crow::response(500, "Unknown error occurred.");
		}
		}); 

	CROW_ROUTE(m_app, "/getPlayer/<string>")([this](const std::string& name) {
		try {
			game_database::GamePlayer dbPlayer = m_db.GetPlayerByName(name);
			Player* player = new Player(dbPlayer);

			std::string response = "Name: " + player->GetName() +
				"\nPoints: " + std::to_string(player->GetPoints()) +
				"\nScore: " + std::to_string(player->GetScore());

			delete player;
			return crow::response(response);

		}
		catch (const std::runtime_error& e) {
			// Captură pentru erorile de tip runtime
			return crow::response(404, std::string(e.what()));
		}
		catch (...) {
			// Captură pentru orice altă eroare neașteptată
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
	
	m_app.port(18080).multithreaded().run();
}
