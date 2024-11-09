#pragma once
#include <vector>
#include <cstdlib>
#include <ctime>


enum class Cell {
	Empty,
	Destructible_Wall,
	Indestructible_Wall,
	Bomb
};

class GameBoard
{
public:
	GameBoard() = default;

	int getRows() const;
	int getCols() const;

	int getBombChance() const;
	int getDestructiblWallChance() const;
	int getIndestructiblWallChance() const;

	void setBombChance(int bombChance);
	void setDestructiblWallChance(int destructiblWallChance);
	void setIndestructiblWallChance(int indestructiblWallChance);

private:
	int m_rows;
	int m_cols;
	std::vector<std::vector<Cell>> m_board;

	int m_bombChance = 0;
	int m_destructiblWallChance = 0;
	int m_indestructiblWallChance = 0;

	bool willDestructibleWallAppear();
	bool willIndestructibleWallAppear();
	bool willBombAppear();

};