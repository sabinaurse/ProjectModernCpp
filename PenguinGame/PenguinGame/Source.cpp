#include "GameManager.h"
#include "Routing.h"
#include <sqlite_orm/sqlite_orm.h>

int main()
{
    GameManager game;
    game_database::PlayerDatabase db;

    Routing r(game, db);
    r.Run();

    return 0;
} 

