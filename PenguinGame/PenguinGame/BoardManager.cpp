#include "BoardManager.h"
#include "Penguin.h"

void BoardManager::InitializeCellTypes()
{
	MapGen::CellTypeDefinition emptyCell = {
		"Empty",
		20,
		nullptr
	};

	MapGen::CellTypeDefinition destructibleWall = {
		"DestructibleWall",
		70,
		nullptr
	};

	MapGen::CellTypeDefinition indestructibleWall = {
		"IndestructibleWall",
		25,
		nullptr
	};

	MapGen::CellTypeDefinition bomb = {
		"HiddenBomb",
		5,
		[](uint32_t x, uint32_t y, const std::vector<std::vector<uint32_t>>& board) {
			return board[x][y] == 1;
		}
	};

	m_gameBoard.AddCellType(0, emptyCell);
	m_gameBoard.AddCellType(1, destructibleWall);
	m_gameBoard.AddCellType(2, indestructibleWall);
	m_gameBoard.AddCellType(3, bomb);
}

void BoardManager::BulletHit(uint32_t x, uint32_t y)
{
	if (x < 0 || x >= m_gameBoard.GetRows() || y < 0 || y >= m_gameBoard.GetCols()) {
		std::cout << "Bullet hit outside the map boundaries.\n";
		return;
	}

	uint8_t cellType = m_gameBoard.GetBoard()[x][y];
	if (cellType == 1) {
		std::cout << "Destructible wall hit at (" << x << ", " << y << ").\n";
		DestroyCell(x, y);
	}
	else if (cellType == 2) {
		std::cout << "Bullet hit indestructible wall at (" << x << ", " << y << ").\n";
	}
	else {
		std::cout << "Bullet hit empty space at (" << x << ", " << y << ").\n";
	}
}

void BoardManager::TriggerExplosion(uint32_t x, uint32_t y, std::uint8_t EXPLOSIONRADIUS)
{
	for (uint32_t i = -EXPLOSIONRADIUS; i <= EXPLOSIONRADIUS; ++i) {
		for (uint32_t j = -EXPLOSIONRADIUS; j <= EXPLOSIONRADIUS; ++j) {
			uint32_t newX = x + i;
			uint32_t newY = y + j;

			if (newX >= 0 && newX < m_gameBoard.GetRows() &&
				newY >= 0 && newY < m_gameBoard.GetCols() &&
				(i * i + j * j <= EXPLOSIONRADIUS * EXPLOSIONRADIUS)) {

				uint8_t cellType = m_gameBoard.GetBoard()[newX][newY];

				if (cellType == 1 || cellType == 2) {
					DestroyCell(newX, newY);
				}

			}
		}
	}
	std::cout << "Explosion triggered at (" << x << ", " << y << ") with radius " << EXPLOSIONRADIUS << ".\n";
}


void BoardManager::DestroyCell(uint32_t x, uint32_t y)
{
	if (x < 0 || x >= m_gameBoard.GetRows() || y < 0 || y >= m_gameBoard.GetCols()) {
		std::cout << "Cannot destroy cell outside map boundaries.\n";
		return;
	}

	m_gameBoard.GetBoard()[x][y] = 0;
	std::cout << "Cell at (" << x << ", " << y << ") destroyed.\n";
}

const MapGen::GameBoard& BoardManager::GetGameBoard() const {
	return m_gameBoard;
}

std::vector<std::pair<uint32_t, uint32_t>> BoardManager::GetStartingPositions() const {
	return m_gameBoard.GetStartingPositions();
}

bool BoardManager::IsWithinBounds(uint32_t x, uint32_t y) const {
	return x >= 0 && x < static_cast<uint32_t>(m_gameBoard.GetRows()) &&
		y >= 0 && y < static_cast<uint32_t>(m_gameBoard.GetCols());
}

int BoardManager::GetCell(uint32_t x, uint32_t y) const {
	if (IsWithinBounds(x, y)) {
		return m_gameBoard.GetBoard()[x][y];
	}
	throw std::out_of_range("Position is out of bounds");
}

crow::json::wvalue BoardManager::SerializeBoard() const {
	crow::json::wvalue json;

	const auto& board = m_gameBoard.GetBoard();
	for (size_t i = 0; i < board.size(); ++i) {
		for (size_t j = 0; j < board[i].size(); ++j) {
			json[i][j] = board[i][j];
		}
	}

	return json;
}

void BoardManager::SetCell(uint32_t x, uint32_t y, uint8_t value) {
	if (IsWithinBounds(x, y)) {
		m_gameBoard.GetBoard()[x][y] = value;
	}
	else {
		throw std::out_of_range("Position is out of bounds");
	}
}
