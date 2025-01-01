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
		30,
		nullptr
	};

	MapGen::CellTypeDefinition bomb = {
		"HiddenBomb",
		5,
		[](uint32_t x, uint32_t y, const std::vector<std::vector<int>>& board) {
			return board[x][y] == 1;
		}
	};

	m_gameBoard.AddCellType(0, emptyCell);
	m_gameBoard.AddCellType(1, destructibleWall);
	m_gameBoard.AddCellType(2, indestructibleWall);
	m_gameBoard.AddCellType(3, bomb);
}

void BoardManager::BulletHit(int x, int y)
{
	if (x < 0 || x >= m_gameBoard.GetRows() || y < 0 || y >= m_gameBoard.GetCols()) {
		std::cout << "Bullet hit outside the map boundaries.\n";
		return;
	}

	int cellType = m_gameBoard.GetBoard()[x][y];
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

void BoardManager::TriggerExplosion(int x, int y, int radius)
{
	for (int i = -radius; i <= radius; ++i) {
		for (int j = -radius; j <= radius; ++j) {
			int newX = x + i;
			int newY = y + j;

			if (newX >= 0 && newX < m_gameBoard.GetRows() &&
				newY >= 0 && newY < m_gameBoard.GetCols() &&
				(i * i + j * j <= radius * radius)) {

				int cellType = m_gameBoard.GetBoard()[newX][newY];

				if (cellType == 1 || cellType == 2) {
					DestroyCell(newX, newY);
				}

			}
		}
	}
	std::cout << "Explosion triggered at (" << x << ", " << y << ") with radius " << radius << ".\n";
}


void BoardManager::DestroyCell(int x, int y)
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

std::vector<std::pair<int, int>> BoardManager::GetStartingPositions() const {
	return m_gameBoard.GetStartingPositions();
}

bool BoardManager::IsWithinBounds(int x, int y) const {
	return x >= 0 && x < static_cast<int>(m_gameBoard.GetRows()) &&
		y >= 0 && y < static_cast<int>(m_gameBoard.GetCols());
}

int BoardManager::GetCell(int x, int y) const {
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

void BoardManager::SetCell(int x, int y, int value) {
	if (IsWithinBounds(x, y)) {
		m_gameBoard.GetBoard()[x][y] = value;
	}
	else {
		throw std::out_of_range("Position is out of bounds");
	}
}

void BoardManager::AddPenguin(std::shared_ptr<Penguin> penguin) {
	m_penguins.push_back(std::move(penguin));
}

void BoardManager::ClearPenguins() {
	m_penguins.clear();
}
