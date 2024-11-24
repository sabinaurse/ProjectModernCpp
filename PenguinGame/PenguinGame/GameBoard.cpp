#include "GameBoard.h"
#include "Penguin.h"
#include <stdexcept>

GameBoard::GameBoard(int rows, int cols, int bombChance, int destructiblWallChance, int indestructiblWallChance, int maxBombs, int minDistanceBombs)
	: m_rows{ rows }, m_cols{ cols },
	m_bombChance{ bombChance }, m_destructiblWallChance{ destructiblWallChance },
	m_indestructiblWallChance{ indestructiblWallChance }, m_maxBombs{ maxBombs },
	m_minDistanceBombs{ minDistanceBombs }
{
	initializeBoard();
}

bool GameBoard::isFarEnoughFromOtherBombs(int x, int y)
{
	for (const auto& bomb : m_bombPositions)
	{
		int dx = bomb.first - x;
		int dy = bomb.second - y;
		double distance = std::sqrt(dx * dx + dy * dy);

		if (distance < m_minDistanceBombs)
		{
			return false;
		}
	}
	return true;
}

void GameBoard::initializeBoard()
{
	m_board.resize(m_rows, std::vector<Cell>(m_cols, Cell::Empty));

	for (int i = 0; i < m_rows; ++i)
		for (int j = 0; j < m_cols; ++j)
		{
			if (willDestructibleWallAppear())
			{
				if (m_bombsPlaced < m_maxBombs && willBombAppear() && isFarEnoughFromOtherBombs(i, j))
				{

					m_board[i][j] = Cell::Hidden_Bomb;
					m_bombPositions.push_back({ i, j });
					m_bombsPlaced++;

				}
				else
				{
					m_board[i][j] = Cell::Destructible_Wall;
				}

			}
			else if (willIndestructibleWallAppear())
			{
				m_board[i][j] = Cell::Indestructible_Wall;
			}

		}
}

std::vector<std::pair<int, int>> GameBoard::GetStartingPositions() {
	std::vector<std::pair<int, int>> startingPositions;

	if (isWithinBounds(0, 0) && getCell(0, 0) == Cell::Empty) {
		startingPositions.emplace_back(0, 0);
	}
	if (isWithinBounds(m_rows - 1, m_cols - 1) && getCell(m_rows - 1, m_cols - 1) == Cell::Empty) {
		startingPositions.emplace_back(m_rows - 1, m_cols - 1);
	}
	if (isWithinBounds(0, m_cols - 1) && getCell(0, m_cols - 1) == Cell::Empty) {
		startingPositions.emplace_back(0, m_cols - 1);
	}
	if (isWithinBounds(m_rows - 1, 0) && getCell(m_rows - 1, 0) == Cell::Empty) {
		startingPositions.emplace_back(m_rows - 1, 0);
	}
	

	if (startingPositions.size() < 4) {
		throw std::runtime_error("Not enough free corners to place players!");
	}

	return startingPositions;
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
			case Cell::Hidden_Bomb:
				std::cout << "B";
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
void GameBoard::destroyCell(int x, int y) {
	if (isWithinBounds(x, y)) {
		m_board[x][y] = Cell::Empty; // Sau altă logică de distrugere
	}
}

void GameBoard::destroyCell(int x, int y, std::vector<Penguin*>& penguins) {
	if (isWithinBounds(x, y))
	{
		Cell currentCell = m_board[x][y];

		if (currentCell == Cell::Destructible_Wall)
		{
			m_board[x][y] = Cell::Empty;
		}
		else if (currentCell == Cell::Hidden_Bomb)
		{
			triggerExplosion(x, y, penguins);
			m_board[x][y] = Cell::Empty;
		}
	}
}


void GameBoard::triggerExplosion(int x, int y, std::vector<Penguin*>& penguins)
{
	const int explosionRadius = 10;

	for (int nx = 0; nx < m_rows; ++nx)
	{
		for (int ny = 0; ny < m_cols; ++ny)
		{
			int dx = nx - x;
			int dy = ny - y;
			double distance = std::sqrt(dx * dx + dy * dy);

			if (distance <= explosionRadius)
			{
					m_board[nx][ny] = Cell::Empty;
			}
		}
	}

	for (Penguin* penguin : penguins) {
		if (!penguin->IsAlive()) continue; // trecem peste pinguinii deja morti

		auto position = penguin->GetPosition();
		int px = position.first;  
		int py = position.second;

		int dx = px - x;
		int dy = py - y;
		double distance = std::sqrt(dx * dx + dy * dy);

		if (distance <= explosionRadius) {
			penguin->ResetCharacter(); // ii luam toate cele 3 vieti pt a-l reseta
			std::cout << "Penguin at position (" << px << ", " << py<< ") was hit by the explosion!" << std::endl;
		}
	}
}

void GameBoard::triggerExplosion(int x, int y) {
	const int explosionRadius = 10;

	for (int nx = 0; nx < m_rows; ++nx) {
		for (int ny = 0; ny < m_cols; ++ny) {
			int dx = nx - x;
			int dy = ny - y;
			double distance = std::sqrt(dx * dx + dy * dy);

			if (distance <= explosionRadius) {
				m_board[nx][ny] = Cell::Empty;
			}
		}
	}
}



void GameBoard::detonateBomb(int x, int y) {
	const int explosionRadius = 10;

	std::vector<Penguin*> emptyPenguins;

	for (int i = 0; i < m_rows; ++i) {
		for (int j = 0; j < m_cols; ++j) {
			int dx = i - x;
			int dy = j - y;
			double distance = std::sqrt(dx * dx + dy * dy);

			if (distance <= explosionRadius) {
				destroyCell(i, j, emptyPenguins); 
			}
		}
	}
}
