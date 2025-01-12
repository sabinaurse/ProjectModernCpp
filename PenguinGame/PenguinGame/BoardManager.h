#pragma once
#include <iostream>
#include <vector>
#include <memory>
#include <array>
#include <cstdlib>
#include <ctime>
#include "crow.h"
#include "../MapGenerationDLL/GameBoard.h"
#include <crow/json.h>

class Penguin;

class BoardManager
{
public:
    BoardManager(uint32_t rows, uint32_t cols) : m_gameBoard(rows, cols)
    {
        InitializeCellTypes();
        m_gameBoard.InitializeBoard();
    }

    void GenerateMap() { m_gameBoard.InitializeBoard(); }
    void DisplayMap() const { m_gameBoard.PrintBoard(); }

    bool IsWithinBounds(int x, int y) const;
    int GetCell(int x, int y) const;

    void BulletHit(int x, int y);
    void TriggerExplosion(int x, int y, int radius);
    void DestroyCell(int x, int y);

    crow::json::wvalue SerializeBoard() const;
    const MapGen::GameBoard& GetGameBoard() const;
    std::vector<std::pair<uint32_t, uint32_t>> GetStartingPositions() const;
    void SetCell(int x, int y, int value);

private:
    void InitializeCellTypes();

private:
    MapGen::GameBoard m_gameBoard;
};

