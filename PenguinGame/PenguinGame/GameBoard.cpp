#include "GameBoard.h"

GameBoard::GameBoard(int rows, int cols, int bombChance, int destructiblWallChance, int indestructiblWallChance)
	: m_rows{ rows }, m_cols{ cols },
	m_bombChance{ bombChance }, m_destructiblWallChance{ destructiblWallChance }, m_indestructiblWallChance{ indestructiblWallChance }
{
	initializeBoard();
}

void GameBoard::initializeBoard()
{
	m_board.resize(m_rows, std::vector<Cell>(m_cols, Cell::Empty));

	for (int i = 0; i < m_rows; ++i)
		for (int j = 0; j < m_cols; ++j)
		{
			if (willDestructibleWallAppear())
				m_board[i][j] = Cell::Destructible_Wall;
			else if (willBombAppear())
				m_board[i][j] = Cell::Bomb;
			else if (willIndestructibleWallAppear())
				m_board[i][j] = Cell::Indestructible_Wall;

		}
}

void GameBoard::printBoard()
{
	for (int i = 0; i < m_rows; ++i)
	{
		for (int j = 0; j < m_cols; ++j)

			switch (m_board[i][j]) {
			case Cell::Empty:
				std::cout << "0";
				break;
			case Cell::Destructible_Wall:
				std::cout << "1";
				break;
			case Cell::Indestructible_Wall:
				std::cout << "2";
				break;
			case Cell::Bomb:
				std::cout << "3";
				break;
			default:
				std::cout << "?";
				break;

			}
		std::cout << std::endl;
	}

}

int GameBoard::getRows() const
{
	return m_rows;
}

int GameBoard::getCols() const
{
	return m_cols;
}

int GameBoard::getBombChance() const
{
	return m_bombChance;
}

int GameBoard::getDestructiblWallChance() const
{
	return m_destructiblWallChance;
}

int GameBoard::getIndestructiblWallChance() const
{
	return m_indestructiblWallChance;
}

void GameBoard::setBombChance(int bombChance)
{
	m_bombChance = bombChance;
}

void GameBoard::setDestructiblWallChance(int destructiblWallChance)
{
	m_destructiblWallChance = destructiblWallChance;
}

void GameBoard::setIndestructiblWallChance(int indestructiblWallChance)
{
	m_indestructiblWallChance = indestructiblWallChance;
}

bool GameBoard::willDestructibleWallAppear()
{
	return std::rand() % 100 < m_destructiblWallChance;
}

bool GameBoard::willIndestructibleWallAppear()
{
	return std::rand() % 100 < m_indestructiblWallChance;
}

bool GameBoard::willBombAppear()
{
	return std::rand() % 100 < m_bombChance;
}

bool GameBoard::isWithinBounds(int x, int y) const 
{
	return x >= 0 && x < m_rows && y >= 0 && y < m_cols;
}

Cell GameBoard::getCell(int x, int y) const
{
	if (isWithinBounds(x, y))
		return m_board[x][y];

	throw std::out_of_range("Coordonatele sunt în afara limitelor tabelei de joc");
}

void GameBoard::setCell(int x, int y, Cell cellType)
{
	if (isWithinBounds(x, y))
		m_board[x][y] = cellType;
	else 
		throw std::out_of_range("Coordonatele sunt în afara limitelor tabelei de joc");
}

void GameBoard::destroyCell(int x, int y)
{
	if (isWithinBounds(x, y) && m_board[x][y] == Cell::Destructible_Wall)
		m_board[x][y] = Cell::Empty;
}
