#include "../MapGenerationDLL/GameBoard.h"
#include <iostream>

int main() {
    try {
        MapGen::GameBoard board(20, 20); 

        board.AddCellType(1, { "Destructible Wall", 50, nullptr }); 
        board.AddCellType(2, { "Indestructible Wall", 30, nullptr }); 
        board.AddCellType(3, { "Bomb", 20, [](uint32_t x, uint32_t y, const std::vector<std::vector<int>>& grid) {
            return (x % 2 == 0 && y % 2 == 0);
        } });

        board.InitializeBoard();

        std::cout << "Harta generata:\n";
        board.PrintBoard();

    }
    catch (const std::exception& e) {
        std::cerr << "Eroare: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}



