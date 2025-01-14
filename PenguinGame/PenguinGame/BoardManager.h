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

constexpr uint8_t EXPLOSIONRADIUS = 10;

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

    bool IsWithinBounds(uint32_t x, uint32_t y) const;
    int GetCell(uint32_t x, uint32_t y) const;

    void BulletHit(uint32_t x, uint32_t y);
    void TriggerExplosion(uint32_t x, uint32_t y, std::uint8_t EXPLOSIONRADIUS);
    void DestroyCell(uint32_t x, uint32_t y);

    crow::json::wvalue SerializeBoard() const;
    const MapGen::GameBoard& GetGameBoard() const;
    std::vector<std::pair<uint32_t, uint32_t>> GetStartingPositions() const;
    void SetCell(uint32_t x, uint32_t y, uint8_t value);

private:
    void InitializeCellTypes();

private:
    MapGen::GameBoard m_gameBoard;
};

