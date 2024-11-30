#include "GameBoard.h"
#include "Game.h"
#include "Routing.h"
#include <sqlite_orm/sqlite_orm.h>

//Empty - 0
//Destructible_Wall - 1
//Indestructible_Wall - 2
//Bomb - 3 

int main() {

    //Testare server

    Game game;
    game_database::PlayerDatabase db;

    Routing r(game, db);
    r.Run();

    /*GameBoard board(50, 50, 30, 50, 5, 3);
    board.PrintBoard();*/

    return 0;
}
