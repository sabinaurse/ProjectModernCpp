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
            throw std::runtime_error("Player already exists in data base");
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
            return result.front();
        }
        else
        {
            throw std::runtime_error("Player not found");
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

    void PlayerDatabase::UpdateCooldownLevel(const std::string& playerName)
    {
        auto players = storage.get_all<GamePlayer>(sqlite_orm::where(sqlite_orm::c(&GamePlayer::name) == playerName));

        if (!players.empty()) {
            GamePlayer& player = players.front();

            if (player.points >= 2000) {
                player.cooldown_level = 4;
            }
            else if (player.points >= 1500) {
                player.cooldown_level = 3;
            }
            else if (player.points >= 1000) {
                player.cooldown_level = 2;
            }
            else if (player.points >= 500) {
                player.cooldown_level = 1;
            }
            else {
                player.cooldown_level = 0;
            }

            storage.update(player);
        }
        else {
            throw std::runtime_error("Player not found");
        }
    }

    void PlayerDatabase::UpdateBulletSpeedLevel(const std::string& playerName)
    {
        auto players = storage.get_all<GamePlayer>(sqlite_orm::where(sqlite_orm::c(&GamePlayer::name) == playerName));

        if (!players.empty()) {
            GamePlayer& player = players.front();

            if (player.score >= 10) {
                player.bullet_speed_level = 1;
            }
            else {
                player.bullet_speed_level = 0;
            }

            storage.update(player);
        }
        else {
            throw std::runtime_error("Player not found");
        }
    }
}