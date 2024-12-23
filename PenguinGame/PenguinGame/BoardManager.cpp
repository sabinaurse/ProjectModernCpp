#include "BoardManager.h"

void BoardManager::InitializeCellTypes()
{
    MapGen::CellTypeDefinition emptyCell = {
        "Empty",
        20,
        nullptr
    };

    MapGen::CellTypeDefinition destructibleWall = {
        "DestructibleWall",
        70,
        nullptr
    };

    MapGen::CellTypeDefinition indestructibleWall = {
        "IndestructibleWall",
        30,
        nullptr
    };

    MapGen::CellTypeDefinition bomb = {
        "HiddenBomb",
        5,
        [](uint32_t x, uint32_t y, const std::vector<std::vector<int>>& board) {
            return board[x][y] == 1;
        }
    };

    m_gameBoard.AddCellType(0, emptyCell);
    m_gameBoard.AddCellType(1, destructibleWall);
    m_gameBoard.AddCellType(2, indestructibleWall);
    m_gameBoard.AddCellType(3, bomb);
}
