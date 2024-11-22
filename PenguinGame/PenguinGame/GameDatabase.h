#pragma once
#include <sqlite_orm/sqlite_orm.h>
#include <string>
#include <vector>

namespace game_database
{
	struct GamePlayer
	{
		int id;
		std::string name;
		std::string password;
		int points;
		int score;
	};

	struct WeaponUpgrade {
		int id;              
		int cooldown;       
		int bullet_speed;    
		int user_id;         
	};

	// Functie pentru crearea structurii bazei de date
	inline auto createStorage(const std::string& filename)
	{
		using namespace sqlite_orm;
		return make_storage(
			filename,
			make_table(
				"players",
				make_column("id", &GamePlayer::id, primary_key().autoincrement()),
				make_column("name", &GamePlayer::name),
				make_column("password", &GamePlayer::password),
				make_column("points", &GamePlayer::points),
				make_column("score", &GamePlayer::score)
			),
			make_table(
				"weapon_upgrades",
				make_column("id", &WeaponUpgrade::id, primary_key().autoincrement()),
				make_column("cooldown", &WeaponUpgrade::cooldown),
				make_column("bullet_speed", &WeaponUpgrade::bullet_speed),
				make_column("user_id", &WeaponUpgrade::user_id)
			)
		);
	}

	using Storage = decltype(createStorage(""));

	// Clasa pentru gestionarea bazei de date a playerului
	class PlayerDatabase {
	public:
		PlayerDatabase();
		
		void AddPlayer(const GamePlayer& player);
		std::vector<GamePlayer> GetAllPlayers();
		GamePlayer GetPlayerByName(const std::string& name);
		void UpdatePlayerPoints(const std::string& name, int newPoints);
		void UpdatePlayerScore(const std::string& name, int newScore);
		void DeletePlayer(const std::string& name);

	private:
		Storage storage;
	};

	class WeaponDatabase {
	public:

		void UpdateFireRate(const std::string& upgradeId, int newBulletSpeed);
		void UpdateCooldown(const std::string& upgradeId, int newCooldown);

	private:
		Storage storage;
	};



}