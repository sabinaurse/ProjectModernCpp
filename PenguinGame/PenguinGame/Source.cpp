#include "GameBoard.h"

//Empty - 0
//Destructible_Wall - 1
//Indestructible_Wall - 2
//Bomb - 3 

void main()
{
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    int minRows = 5, maxRows = 10;
    int minCols = 5, maxCols = 10;

    int randomRows = minRows + std::rand() % (maxRows - minRows + 1);
    int randomCols = minCols + std::rand() % (maxCols - minCols + 1);

    int bombChance = 15;
    int destructiblWallChance = 30;
    int indestructiblWallChance = 30;
    int maxBombs = 3;
    int distanceBombs = 3; // doar pt testare

    GameBoard board(randomRows, randomCols, bombChance, destructiblWallChance, indestructiblWallChance, maxBombs, distanceBombs);

    board.printBoard();

}