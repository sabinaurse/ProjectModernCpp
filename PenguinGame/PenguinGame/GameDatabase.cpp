#include "GameDatabase.h"

namespace game_database
{
	PlayerDatabase::PlayerDatabase() : storage(createStorage("PenguinGame.sqlite"))
	{
		storage.sync_schema(); // Sincronizeaza schema bazei de date
	}

	void PlayerDatabase::AddPlayer(const GamePlayer& player)
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

	std::vector<GamePlayer> PlayerDatabase::GetAllPlayers()
	{
		return storage.get_all<GamePlayer>();
	}

	GamePlayer PlayerDatabase::GetPlayerByName(const std::string& name)
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

	void PlayerDatabase::UpdatePlayerScore(const std::string& name, int newScore)
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

	void PlayerDatabase::UpdatePlayerPoints(const std::string& name, int newPoints)
	{
		auto players = storage.get_all<GamePlayer>(sqlite_orm::where(sqlite_orm::c(&GamePlayer::name) == name));

		if (!players.empty())
		{
			GamePlayer player = players.front();
			player.points = newPoints;
			storage.update(player);
		}
		else
		{
			throw std::runtime_error("Player not found");
		}
	}

	void PlayerDatabase::DeletePlayer(const std::string& name) {
		try {
			
			auto players = storage.get_all<GamePlayer>(sqlite_orm::where(sqlite_orm::c(&GamePlayer::name) == name));

			if (players.empty()) {
				throw std::runtime_error("Player not found");
			}

			auto player = players.front();
			storage.remove<GamePlayer>(player.id);

		}
		catch (const std::runtime_error& e) {
			throw std::runtime_error("Failed to delete player: " + std::string(e.what()));
		}
	}

	void WeaponDatabase::UpdateFireRate(const std::string& upgradeId, int newBulletSpeed)
	{
		auto upgrade = storage.get<WeaponUpgrade>(sqlite_orm::where(sqlite_orm::c(&WeaponUpgrade::id) == std::stoi(upgradeId)));
		upgrade.bullet_speed = newBulletSpeed;
		storage.update(upgrade);
	}

	void WeaponDatabase::UpdateCooldown(const std::string& upgradeId, int newCooldown)
	{
		auto upgrade = storage.get<WeaponUpgrade>(sqlite_orm::where(sqlite_orm::c(&WeaponUpgrade::id) == std::stoi(upgradeId)));
		upgrade.cooldown = newCooldown;
		storage.update(upgrade);
	}

}