//#include "Game.h"
//#include "Routing.h"
//#include <sqlite_orm/sqlite_orm.h>
//
//int main()
//{
//    uint32_t rows = 30;
//    uint32_t cols = 30;
//
//    Game game(rows, cols);
//    game_database::PlayerDatabase db;
//
//    Routing r(game, db);
//    r.Run();
//
//    return 0;
//} 

#include "GameManager.h"
#include "Player.h"
#include <iostream>
#include <vector>
#include <cstdlib>
#include <thread>
#include <chrono>

int main() {
    GameManager gameManager;

    // Adăugăm trei jucători în coadă
    Player* player1 = new Player("Player1", 50);
    Player* player2 = new Player("Player2", 30);
    Player* player3 = new Player("Player3", 70);

    std::cout << "[Main] Adauga Player1 în coada cu scorul 50" << std::endl;
    gameManager.AddPlayerToQueue(player1);

    std::cout << "[Main] Adauga Player2 în coada cu scorul 30" << std::endl;
    gameManager.AddPlayerToQueue(player2);

    std::cout << "[Main] Adauga Player3 în coada cu scorul 70" << std::endl;
    gameManager.AddPlayerToQueue(player3);

    // Pornim bucla de multigaming
    for (int i = 0; i < 10; ++i) {
        std::cout << "[Main] Iteratia " << i + 1 << " a buclei principale" << std::endl;
        gameManager.RunMultigamingLoop();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

   // std::cout << "[Main] Toți jucătorii au fost procesați. Jocul s-a terminat." << std::endl;

    return 0;
}