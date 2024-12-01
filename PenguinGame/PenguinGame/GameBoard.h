#pragma once
#include <iostream>
#include <vector>
#include <array>
#include <cstdlib>
#include <ctime>
#include "crow.h"

using Position = std::pair<std::uint32_t, std::uint32_t>;

class Penguin;

enum class Cell {
	Empty,               //0
	Destructible_Wall,   //1
	Indestructible_Wall, //2
	Hidden_Bomb          //3 
};

class GameBoard
{
public:
	GameBoard() = default;
	GameBoard(uint16_t bombChance, uint16_t destructibleWallChance, uint16_t indestructibleWallChance, uint16_t emptyCellChance, uint16_t maxBombs, uint16_t minDistanceBombs);

	void InitializeBoard();
	crow::json::wvalue SerializeBoard() const;
	void PrintBoard();
	uint32_t GetRows() const;
	uint32_t GetCols() const;

	int GetBombChance() const;
	int GetDestructiblWallChance() const;
	int GetIndestructiblWallChance() const;

	bool IsWithinBounds(uint32_t x, uint32_t y) const;
	Cell GetCell(uint32_t x, uint32_t  y) const;
	void SetCell(uint32_t x, uint32_t  y, Cell cellType);
	void DestroyCell(uint32_t  x, uint32_t  y, std::vector<Penguin*>& penguins);
	void DestroyCell(uint32_t  x, uint32_t y);
	std::vector<Position> GetStartingPositions();

	void DetonateBomb(uint32_t x, uint32_t y);
	void TriggerExplosion(uint32_t x, uint32_t y, std::vector<Penguin*>& penguins);
	void TriggerExplosion(uint32_t x, uint32_t y);

	void CreatePathsBetweenCorners();
	void CreatePath(Position start, Position end);
	bool AreCornersConnected() const;

private:
	bool IsFarEnoughFromOtherBombs(uint32_t x, uint32_t y);
	bool WillDestructibleWallAppear();
	bool WillIndestructibleWallAppear();
	bool WillBombAppear();
	bool WillCellBeEmptyAppear();

private:
	static constexpr int m_rows{ 10 };
	static constexpr int m_cols{ 10 };
	std::array<std::array<Cell, m_rows>, m_cols> m_board;
	std::vector<Position> m_bombPositions;

	uint16_t m_bombChance{ 0 };
	uint16_t m_cellEmptyChance{ 0 };
	uint16_t m_destructiblWallChance{ 0 };
	uint16_t m_indestructiblWallChance{ 0 };
	uint16_t m_bombsPlaced{ 0 };
	uint16_t m_maxBombs{ 0 };
	uint16_t m_minDistanceBombs{ 0 };
};