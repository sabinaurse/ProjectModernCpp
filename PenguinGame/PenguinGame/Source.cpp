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

    int bombChance = 10;
    int destructiblWallChance = 20;
    int indestructiblWallChance = 30;

    GameBoard board(randomRows, randomCols, bombChance, destructiblWallChance, indestructiblWallChance);

    board.printBoard();

}