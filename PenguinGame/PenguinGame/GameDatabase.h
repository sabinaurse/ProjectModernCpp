﻿#pragma once
#include <sqlite_orm/sqlite_orm.h>
#include <string>
#include <vector>

namespace game_database
{
	struct GamePlayer
	{
		int id;
		std::string name;
		int points;
		int score;
		int bullet_speed_level;
		int cooldown_level;
	};

	struct WeaponUpgrade { //-> de eliminat
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
				make_column("points", &GamePlayer::points),
				make_column("score", &GamePlayer::score),
				make_column("bullet_speed_level", &GamePlayer::bullet_speed_level, default_value(0)),
				make_column("cooldown_level", &GamePlayer::cooldown_level, default_value(0))
			),
			make_table(
				"weapon_upgrades", //-> de eliminat aceasta tabela
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

		void UpdateFireRate(const std::string& upgradeId, int newBulletSpeed); //-> de refacut pe baza scorului/punctelor + mutate in PlayerDatabase
		void UpdateCooldown(const std::string& upgradeId, int newCooldown); //-> de refacut pe baza scorului/punctelor + mutate in PlayerDatabase

	private:
		Storage storage;
	};



}