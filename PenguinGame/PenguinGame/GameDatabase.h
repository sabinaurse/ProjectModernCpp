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
        int points;
        int score;
        int bullet_speed_level;
        int cooldown_level;
        //id_game => multigaming
    };

    inline auto createStorage(const std::string& filename)
    {
        using namespace sqlite_orm;
        return make_storage(
            filename,
            make_table(
                "players",
                make_column("id", &GamePlayer::id, primary_key().autoincrement()),
                make_column("name", &GamePlayer::name),
                make_column("points", &GamePlayer::points, default_value(0)),
                make_column("score", &GamePlayer::score, default_value(0)),
                make_column("bullet_speed_level", &GamePlayer::bullet_speed_level, default_value(0)),
                make_column("cooldown_level", &GamePlayer::cooldown_level, default_value(0))
            )
        );
    }

    using Storage = decltype(createStorage(""));

    class PlayerDatabase {
    public:
        PlayerDatabase();

        void AddPlayer(const GamePlayer& player);
        std::vector<GamePlayer> GetAllPlayers();
        GamePlayer GetPlayerByName(const std::string& name);
        void UpdatePlayerPoints(const std::string& name, int newPoints);
        void UpdatePlayerScore(const std::string& name, int newScore);
        void UpdateCooldownLevel(const std::string& playerName);
        void UpdateBulletSpeedLevel(const std::string& playerName);

    private:
        Storage storage;
    };
}