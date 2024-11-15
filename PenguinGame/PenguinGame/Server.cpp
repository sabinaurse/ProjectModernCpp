#include <crow.h>
#include "Game.h"

int main()
{
	crow::SimpleApp app; // Instanta de aplicatie Crow

	Game game; // Creeaza o instanta a clasei joc

	// Test route pentru a verifica functionarea serverului
	CROW_ROUTE(app, "/")([]() {
		return "Server is running!";
		});

	// Ruta pentru adaugarea unui jucator
	CROW_ROUTE(app, "/addPlayer/<string>/<string>")([&game](const std::string& name, const std::string& password) {
		Player* newPlayer = new Player(name, password);
		game.AddPlayer(newPlayer);

		return crow::response("Player added: " + name);
		});

	// Porneste serverul pe portalul 18080
	app.port(18080).multithreaded().run();

	return 0;
}