#pragma once
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

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
	GameBoard(int rows, int cols, int bombChance, int destructibleWallChance, int indestructibleWallChance, int emptyCellChance, int maxBombs, int minDistanceBombs);

	void InitializeBoard();
	void PrintBoard();
	int GetRows() const;
	int GetCols() const;

	int GetBombChance() const;
	int GetDestructiblWallChance() const;
	int GetIndestructiblWallChance() const;

	bool IsWithinBounds(int x, int y) const;
	Cell GetCell(int x, int y) const;
	void SetCell(int x, int y, Cell cellType);
	void DestroyCell(int x, int y, std::vector<Penguin*>& penguins);
	void DestroyCell(int x, int y); 
	std::vector<std::pair<int, int>> GetStartingPositions();

	void DetonateBomb(int x, int y);
	void TriggerExplosion(int x, int y, std::vector<Penguin*>& penguins);
	void TriggerExplosion(int x, int y);

	void CreatePathsBetweenCorners();
	void CreatePath(std::pair<int, int> start, std::pair<int, int> end);
	bool AreCornersConnected() const;

private:
	int m_rows = 0;
	int m_cols = 0;
	std::vector<std::vector<Cell>> m_board;
	std::vector<std::pair<int, int>> m_bombPositions;

	int m_bombChance = 0;
	int m_cellEmptyChange = 0;
	int m_destructiblWallChance = 0;
	int m_indestructiblWallChance = 0;
	int m_bombsPlaced = 0;
	int m_maxBombs = 0;
	int m_minDistanceBombs = 0;

	bool isFarEnoughFromOtherBombs(int x, int y);
	bool willDestructibleWallAppear();
	bool willIndestructibleWallAppear();
	bool willBombAppear();
	bool willCellBeEmptyAppear();

};