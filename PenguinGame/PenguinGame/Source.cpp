#include "Game.h"
#include "Routing.h"
#include <sqlite_orm/sqlite_orm.h>

int main()
{
    uint32_t rows = 20;
    uint32_t cols = 20;

    BoardManager boardManager(rows, cols);
    std::cout << "Harta generata:\n";
    boardManager.DisplayMap();

    Game game(rows, cols);
    game_database::PlayerDatabase db;

    Routing r(game, db);
    r.Run();

    return 0;
} 

