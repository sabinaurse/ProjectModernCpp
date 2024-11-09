#include "GameBoard.h"

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