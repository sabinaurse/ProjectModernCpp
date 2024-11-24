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
	GameBoard(int rows, int cols, int bombChance, int destructiblWallChance, int indestructiblWallChance, int maxBombs, int minDistanceBombs);

	void  initializeBoard();
	void printBoard();

	int getRows() const;
	int getCols() const;

	int getBombChance() const;
	int getDestructiblWallChance() const;
	int getIndestructiblWallChance() const;

	bool isWithinBounds(int x, int y) const;
	Cell getCell(int x, int y) const;
	void setCell(int x, int y, Cell cellType);
	void destroyCell(int x, int y, std::vector<Penguin*>& penguins);
	void destroyCell(int x, int y); 
	std::vector<std::pair<int, int>> GetStartingPositions();

	void detonateBomb(int x, int y);
	void triggerExplosion(int x, int y, std::vector<Penguin*>& penguins);
	void triggerExplosion(int x, int y);

private:
	int m_rows = 0;
	int m_cols = 0;
	std::vector<std::vector<Cell>> m_board;
	std::vector<std::pair<int, int>> m_bombPositions;

	int m_bombChance = 0;
	int m_destructiblWallChance = 0;
	int m_indestructiblWallChance = 0;
	int m_bombsPlaced = 0;
	int m_maxBombs = 0;
	int m_minDistanceBombs = 0;

	bool isFarEnoughFromOtherBombs(int x, int y);
	bool willDestructibleWallAppear();
	bool willIndestructibleWallAppear();
	bool willBombAppear();

};