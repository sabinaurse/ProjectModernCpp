#include "MapGeneration.h"
#include "pch.h"
#include <cstdlib>
#include <ctime>

// Funcție care generează o hartă aleatorie
extern "C" __declspec(dllexport) Map GenerateRandomMap(uint32_t rows, uint32_t cols,
    uint16_t wallChance, uint16_t bombChance,
    uint16_t emptyChance) {
    Map newMap;
    newMap.rows = rows;
    newMap.cols = cols;
    newMap.grid.resize(rows, std::vector<Cell>(cols, Cell::Empty));

    std::srand(static_cast<unsigned int>(std::time(0)));

    // Generare aleatorie a celulelor
    for (uint32_t i = 0; i < rows; ++i) {
        for (uint32_t j = 0; j < cols; ++j) {
            int randValue = std::rand() % 100;

            if (randValue < wallChance) {
                newMap.grid[i][j] = Cell::Destructible_Wall;
            }
            else if (randValue < wallChance + 10) {
                newMap.grid[i][j] = Cell::Indestructible_Wall;
            }
            else if (randValue < wallChance + bombChance) {
                newMap.grid[i][j] = Cell::Bomb;
            }
            // Dacă nu este nici zid, nici bombă, lasăm celula goală
        }
    }

    return newMap;
}

// Funcție pentru a crea drumuri libere între colțuri
extern "C" __declspec(dllexport) void CreatePaths(Map& gameMap) {
    std::vector<std::pair<int, int>> corners = {
        {0, 0}, {0, gameMap.cols - 1}, {gameMap.rows - 1, gameMap.cols - 1}, {gameMap.rows - 1, 0}
    };

    for (size_t i = 0; i < corners.size(); ++i) {
        auto start = corners[i];
        auto end = corners[(i + 1) % corners.size()];
        CreatePath(gameMap, start, end);
    }
}

// Funcție auxiliară pentru a crea un drum între două puncte
void CreatePath(Map& gameMap, std::pair<int, int> start, std::pair<int, int> end) {
    int x = start.first, y = start.second;
    int targetX = end.first, targetY = end.second;

    // Creăm drum între cele două puncte
    while (x != targetX || y != targetY) {
        gameMap.grid[x][y] = Cell::Empty; // Marcăm celula ca drum liber
        if (x != targetX) {
            x += (x < targetX) ? 1 : -1;
        }
        else if (y != targetY) {
            y += (y < targetY) ? 1 : -1;
        }
    }
    gameMap.grid[x][y] = Cell::Empty; // Ultima celulă
}

// Funcție pentru a seta o celulă
extern "C" __declspec(dllexport) void SetCell(Map& gameMap, uint32_t x, uint32_t y, Cell cellType) {
    if (x < gameMap.rows && y < gameMap.cols) {
        gameMap.grid[x][y] = cellType;
    }
}
