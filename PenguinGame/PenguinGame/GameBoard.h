#pragma once
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>


enum class Cell {
	Empty,               //0
	Destructible_Wall,   //1
	Indestructible_Wall, //2
	Bomb                 //3 <- ulterior trebuie modificata astfel incat sa fie "in spatele unui zid destructibil"
};

class GameBoard
{
public:
	GameBoard() = default;
	GameBoard(int rows, int cols, int bombChance, int destructiblWallChance, int indestructiblWallChance);

	void  initializeBoard();
	void printBoard();

	int getRows() const;
	int getCols() const;

	int getBombChance() const;
	int getDestructiblWallChance() const;
	int getIndestructiblWallChance() const;

	void setBombChance(int bombChance);
	void setDestructiblWallChance(int destructiblWallChance);
	void setIndestructiblWallChance(int indestructiblWallChance);
	
	bool isWithinBounds(int x, int y) const;
	Cell getCell(int x, int y) const;
	void setCell(int x, int y, Cell cellType);
	void destroyCell(int x, int y);
private:
	int m_rows = 0;
	int m_cols = 0;
	std::vector<std::vector<Cell>> m_board;

	int m_bombChance = 0;
	int m_destructiblWallChance = 0;
	int m_indestructiblWallChance = 0;

	bool willDestructibleWallAppear();
	bool willIndestructibleWallAppear();
	bool willBombAppear();

};