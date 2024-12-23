#pragma once
#include <iostream>
#include <vector>
#include <array>
#include <cstdlib>
#include <ctime>
#include "crow.h"
#include "../MapGenerationDLL/GameBoard.h"
#include "Penguin.h"

class BoardManager
{
public:
    BoardManager(uint32_t rows, uint32_t cols): m_gameBoard(rows, cols) 
    {
        InitializeCellTypes();
    }

    void GenerateMap() { m_gameBoard.InitializeBoard(); }
    void DisplayMap() const { m_gameBoard.PrintBoard(); }

    void BulletHit(int x, int y);
    void TriggerExplosion(int x, int y, int radius);
    void CheckPlayersInExplosion(int x, int y);
    void InitializePenguins(const std::vector<Player*>& players, const std::vector<Position>& startPositions);

private:
    void InitializeCellTypes();
    void DestroyCell(int x, int y);

private:
    MapGen::GameBoard m_gameBoard;
    std::vector<Penguin> m_penguins;
};

