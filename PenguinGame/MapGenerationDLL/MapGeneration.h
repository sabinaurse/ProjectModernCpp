#pragma once
#include <vector>

enum class Cell {
    Empty,               // 0
    Destructible_Wall,   // 1
    Indestructible_Wall, // 2
    Bomb,                // 3
};

struct Map {
    std::vector<std::vector<Cell>> grid;
    uint32_t rows;
    uint32_t cols;
};

// Funcție pentru generarea aleatorie a hărții
extern "C" __declspec(dllexport) Map GenerateRandomMap(uint32_t rows, uint32_t cols,
    uint16_t wallChance, uint16_t bombChance,
    uint16_t emptyChance);

// Funcție pentru a crea drumuri libere pe hartă
extern "C" __declspec(dllexport) void CreatePaths(Map& gameMap);

// Funcție pentru a seta o celulă pe hartă
extern "C" __declspec(dllexport) void SetCell(Map& gameMap, uint32_t x, uint32_t y, Cell cellType);
