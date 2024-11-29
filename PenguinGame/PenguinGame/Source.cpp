#include "GameBoard.h"
#include "Game.h"
#include "Routing.h"
#include <sqlite_orm/sqlite_orm.h>

//Empty - 0
//Destructible_Wall - 1
//Indestructible_Wall - 2
//Bomb - 3 

int main() {

    ////Testare server

    //Game game;
    //game_database::PlayerDatabase db;

    //Routing r(game, db);
    //r.Run();

    ////Testare clasa Game

    //// Creăm jucători
    //Player player1("Alice");
    //Player player2("Bob");

    //// Adăugăm jucători în joc
    //game.AddPlayer(&player1);  // Adăugăm jucătorul în joc
    //game.AddPlayer(&player2);

    //// Începem jocul
    //game.StartGame();

    //// Accesăm pinguinul pentru fiecare jucător
    //for (auto* player : game.GetPlayers()) {  // folosim pointeri aici
    //    std::cout << "Trying to get penguin for player: " << player->GetName() << std::endl;
    //    Penguin* penguin = game.GetPenguinForPlayer(*player);
    //    if (penguin) {
    //        std::cout << player->GetName() << " controls a penguin at position ("
    //            << penguin->GetPosition().first << ", " << penguin->GetPosition().second << ")" << std::endl;
    //    }
    //    else {
    //        std::cout << "No penguin found for player: " << player->GetName() << std::endl;
    //    }
    //}

    GameBoard board(15, 15, 50, 50, 30,50, 5, 3);
    board.PrintBoard();

    return 0;
}
