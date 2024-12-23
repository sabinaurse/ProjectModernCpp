#pragma once
#include <iostream>
#include <vector>
#include <array>
#include <cstdlib>
#include <ctime>
#include "crow.h"
#include "../MapGenerationDLL/GameBoard.h"

class BoardManager
{
public:
    BoardManager(uint32_t rows, uint32_t cols)
        : m_gameBoard(rows, cols) {
        InitializeCellTypes();
    }

    void GenerateMap() {
        m_gameBoard.InitializeBoard();
    }

    void DisplayMap() const {
        m_gameBoard.PrintBoard();
    }

private:
    void InitializeCellTypes();

private:
    MapGen::GameBoard m_gameBoard;
};

