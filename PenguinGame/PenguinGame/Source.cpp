#include "GameBoard.h"
#include "Game.h"

//Empty - 0
//Destructible_Wall - 1
//Indestructible_Wall - 2
//Bomb - 3 

//void main()
//{
//    std::srand(static_cast<unsigned int>(std::time(nullptr)));
//
//    int minRows = 5, maxRows = 10;
//    int minCols = 5, maxCols = 10;
//
//    int randomRows = minRows + std::rand() % (maxRows - minRows + 1);
//    int randomCols = minCols + std::rand() % (maxCols - minCols + 1);
//
//    int bombChance = 15;
//    int destructiblWallChance = 30;
//    int indestructiblWallChance = 30;
//    int maxBombs = 3;
//    int distanceBombs = 3; // doar pt testare
//
//    GameBoard board(randomRows, randomCols, bombChance, destructiblWallChance, indestructiblWallChance, maxBombs, distanceBombs);
//
//    board.printBoard();
//
//}


//int main() {
//    Game game;
//
//    // Creăm jucători
//    Player player1("Alice", "password123");
//    Player player2("Bob", "securepass");
//
//    // Adăugăm jucători în joc
//    game.AddPlayer(&player1);  // Adăugăm jucătorul în joc
//    game.AddPlayer(&player2);
//
//    // Începem jocul
//    game.StartGame();
//
//    // Accesăm pinguinul pentru fiecare jucător
//    for (auto* player : game.GetPlayers()) {  // folosim pointeri aici
//        std::cout << "Trying to get penguin for player: " << player->GetName() << std::endl;
//        Penguin* penguin = game.GetPenguinForPlayer(*player);
//        if (penguin) {
//            std::cout << player->GetName() << " controls a penguin at position ("
//                << penguin->GetPosition().first << ", " << penguin->GetPosition().second << ")" << std::endl;
//        }
//        else {
//            std::cout << "No penguin found for player: " << player->GetName() << std::endl;
//        }
//    }
//
//    return 0;
//}
