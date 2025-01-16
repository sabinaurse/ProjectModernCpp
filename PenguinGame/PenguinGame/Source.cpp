#include "Game.h"
#include "Routing.h"
#include <sqlite_orm/sqlite_orm.h>

int main()
{
    uint32_t rows = 30;
    uint32_t cols = 30;

    Game game(rows, cols);
    game_database::PlayerDatabase db;

    Routing r(game, db);
    r.Run();

    return 0;
} 


