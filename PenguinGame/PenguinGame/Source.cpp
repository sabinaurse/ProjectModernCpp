#include <iostream>
#include "BoardManager.h"

int main() {
    BoardManager board(20, 25);

    board.GenerateMap();
    std::cout << "Harta generata:\n";
    board.DisplayMap();

    return 0;
}


