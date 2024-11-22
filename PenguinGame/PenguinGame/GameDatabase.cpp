#include "GameDatabase.h"

namespace game_database
{
	Database::Database() : storage(createStorage("PenguinGame.sqlite"))
	{
		storage.sync_schema(); // Sincronizeaza schema bazei de date
	}

	void Database::AddPlayer(const GamePlayer& player)
	{
		auto result = storage.get_all<GamePlayer>(sqlite_orm::where(sqlite_orm::c(&GamePlayer::name) == player.name));

		if (result.empty())
		{
			storage.insert(player); 
		}
		else
		{
			throw std::runtime_error("Player already exists in data base"); // Gestionare eroare daca exista deja numele player-ului in baza de date
		}
	}

	std::vector<GamePlayer> Database::GetAllPlayers()
	{
		return storage.get_all<GamePlayer>();
	}

	GamePlayer Database::GetPlayerByName(const std::string& name)
	{
		auto result = storage.get_all<GamePlayer>(sqlite_orm::where(sqlite_orm::c(&GamePlayer::name) == name));

		if (!result.empty())
		{
			return result.front(); // Returneaza primul rezultat daca exista
		}
		else
		{
			throw std::runtime_error("Player not found"); // Gestionare eroare daca exista
		}
	}

	void Database::UpdatePlayerScore(const std::string& name, int newScore)
	{
		auto players = storage.get_all<GamePlayer>(sqlite_orm::where(sqlite_orm::c(&GamePlayer::name) == name));

		if (!players.empty())
		{
			GamePlayer player = players.front();
			player.score = newScore;
			storage.update(player);
		}
		else
		{
			throw std::runtime_error("Player not found");
		}
	}
}