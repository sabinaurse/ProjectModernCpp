#include "GameBoard.h"
#include "Penguin.h"
#include <stdexcept>

GameBoard::GameBoard(uint16_t bombChance, uint16_t destructiblWallChance, uint16_t indestructiblWallChance, uint16_t emptyCellChance, uint16_t maxBombs, uint16_t minDistanceBombs)
	: m_bombChance{ bombChance }, m_destructiblWallChance{ destructiblWallChance },
	m_indestructiblWallChance{ indestructiblWallChance }, m_cellEmptyChance(emptyCellChance), m_maxBombs{ maxBombs },
	m_minDistanceBombs{ minDistanceBombs }
{
	std::srand(static_cast<unsigned int>(std::time(0)) ^ std::rand());
	InitializeBoard();
}

void GameBoard::InitializeBoard()
{
	m_bombPositions.clear();
	m_bombsPlaced = 0;

	for (uint32_t i = 0; i < m_rows; ++i) {
		for (uint32_t j = 0; j < m_cols; ++j) {
			m_board[i][j] = Cell::Indestructible_Wall;
		}
	}

	// Creăm drumuri libere între colțuri
	CreatePathsBetweenCorners();

	for (uint32_t i = 0; i < m_rows; ++i)
		for (uint32_t j = 0; j < m_cols; ++j)
		{
			// Evităm să suprascriem drumurile libere
			if (m_board[i][j] == Cell::Empty) {
				continue;
			}

			if (WillCellBeEmptyAppear()) {
				m_board[i][j] = Cell::Empty;
			}
			else if (WillDestructibleWallAppear())
			{
				if (m_bombsPlaced < m_maxBombs && WillBombAppear() && IsFarEnoughFromOtherBombs(i, j))
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
			else if (WillIndestructibleWallAppear())
			{
				m_board[i][j] = Cell::Indestructible_Wall;
			}

		}
	// Verificăm dacă colțurile sunt conectate
	if (!AreCornersConnected()) {
		throw std::runtime_error("The generated map does not have connected corners!");
	}
}

void GameBoard::PrintBoard()
{
	for (uint32_t i = 0; i < m_rows; ++i)
	{
		for (uint32_t j = 0; j < m_cols; ++j)

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

crow::json::wvalue GameBoard::SerializeBoard() const
{
	crow::json::wvalue jsonBoard;

	jsonBoard["rows"] = m_rows;
	jsonBoard["cols"] = m_cols;

	auto& cells = jsonBoard["cells"];
	for (uint32_t i = 0; i < m_rows; ++i) {
		for (uint32_t j = 0; j < m_cols; ++j) {
			cells[i][j] = static_cast<uint32_t>(m_board[i][j]);
		}
	}

	auto& bombs = jsonBoard["bombs"];
	int bombIndex = 0;
	for (const auto& bomb : m_bombPositions) {
		bombs[bombIndex++] = crow::json::wvalue{
			{"x", bomb.first},
			{"y", bomb.second}
		};
	}

	return jsonBoard;
}



uint32_t GameBoard::GetRows() const
{
	return m_rows;
}

uint32_t GameBoard::GetCols() const
{
	return m_cols;
}

int GameBoard::GetBombChance() const
{
	return m_bombChance;
}

int GameBoard::GetDestructiblWallChance() const
{
	return m_destructiblWallChance;
}

int GameBoard::GetIndestructiblWallChance() const
{
	return m_indestructiblWallChance;
}


bool GameBoard::IsWithinBounds(uint32_t x, uint32_t y) const
{
	return x >= 0 && x < m_rows && y >= 0 && y < m_cols;
}

Cell GameBoard::GetCell(uint32_t x, uint32_t y) const
{
	if (IsWithinBounds(x, y))
		return m_board[x][y];

	throw std::out_of_range("Coordonatele sunt în afara limitelor tabelei de joc");
}

void GameBoard::SetCell(uint32_t x, uint32_t y, Cell cellType)
{
	if (IsWithinBounds(x, y))
		m_board[x][y] = cellType;
	else
		throw std::out_of_range("Coordonatele sunt în afara limitelor tabelei de joc");
}


void GameBoard::DestroyCell(uint32_t x, uint32_t y) {
	if (IsWithinBounds(x, y)) {
		m_board[x][y] = Cell::Empty; // Sau altă logică de distrugere
	}
}

void GameBoard::DestroyCell(uint32_t x, uint32_t y, std::vector<Penguin*>& penguins) {
	if (IsWithinBounds(x, y))
	{
		Cell currentCell = m_board[x][y];

		if (currentCell == Cell::Destructible_Wall)
		{
			m_board[x][y] = Cell::Empty;
		}
		else if (currentCell == Cell::Hidden_Bomb)
		{
			TriggerExplosion(x, y, penguins);
			m_board[x][y] = Cell::Empty;
		}
	}
}

/*std::vector<std::pair<int, int>> GameBoard::GetStartingPositions() {
	std::vector<std::pair<int, int>> startingPositions;

	if (IsWithinBounds(0, 0) && GetCell(0, 0) == Cell::Empty) {
		startingPositions.emplace_back(0, 0);
	}
	if (IsWithinBounds(m_rows - 1, m_cols - 1) && GetCell(m_rows - 1, m_cols - 1) == Cell::Empty) {
		startingPositions.emplace_back(m_rows - 1, m_cols - 1);
	}
	if (IsWithinBounds(0, m_cols - 1) && GetCell(0, m_cols - 1) == Cell::Empty) {
		startingPositions.emplace_back(0, m_cols - 1);
	}
	if (IsWithinBounds(m_rows - 1, 0) && GetCell(m_rows - 1, 0) == Cell::Empty) {
		startingPositions.emplace_back(m_rows - 1, 0);
	}


	if (startingPositions.size() < 4) {
		throw std::runtime_error("Not enough free corners to place players!");
	}

	return startingPositions;
}*/

std::vector<Position> GameBoard::GetStartingPositions() {
	std::vector<Position> startingPositions = {
		{0, 0}, {0, m_cols - 1}, {m_rows - 1, m_cols - 1}, {m_rows - 1, 0}
	};

	for (const auto& pos : startingPositions) {
		if (!IsWithinBounds(pos.first, pos.second) || GetCell(pos.first, pos.second) != Cell::Empty) {
			throw std::runtime_error("Invalid starting position found!");
		}
	}

	return startingPositions;
}

void GameBoard::DetonateBomb(uint32_t x, uint32_t y) {
	const int explosionRadius = 10;

	std::vector<Penguin*> emptyPenguins;

	for (uint32_t i = 0; i < m_rows; ++i) {
		for (uint32_t j = 0; j < m_cols; ++j) {
			int dx = i - x;
			int dy = j - y;
			double distanceSquared = dx * dx + dy * dy;

			if (distanceSquared <= explosionRadius* explosionRadius) {
				DestroyCell(i, j, emptyPenguins);
			}
		}
	}
}

void GameBoard::TriggerExplosion(uint32_t x, uint32_t y, std::vector<Penguin*>& penguins)
{
	const int explosionRadius = 10;

	for (uint32_t nx = 0; nx < m_rows; ++nx)
	{
		for (uint32_t ny = 0; ny < m_cols; ++ny)
		{
			uint32_t dx = nx - x;
			uint32_t dy = ny - y;
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
		uint32_t px = position.first;
		uint32_t py = position.second;

		uint32_t dx = px - x;
		uint32_t dy = py - y;
		double distance = std::sqrt(dx * dx + dy * dy);

		if (distance <= explosionRadius) {
			penguin->ResetState(); // ii luam toate cele 3 vieti pt a-l reseta
			std::cout << "Penguin at position (" << px << ", " << py << ") was hit by the explosion!" << std::endl;
		}
	}
}

void GameBoard::TriggerExplosion(uint32_t x, uint32_t y, std::vector<std::unique_ptr<Penguin>>& penguins)
{
	const int explosionRadius = 10;

	for (uint32_t nx = 0; nx < m_rows; ++nx) {
		for (uint32_t ny = 0; ny < m_cols; ++ny) {
			uint32_t dx = nx > x ? nx - x : x - nx;
			uint32_t dy = ny > y ? ny - y : y - ny;
			double distance = std::sqrt(dx * dx + dy * dy);

			if (distance <= explosionRadius) {
				m_board[nx][ny] = Cell::Empty;
			}
		}
	}

	for (const auto& penguin : penguins) {
		if (!penguin->IsAlive()) continue;

		auto position = penguin->GetPosition();
		uint32_t px = position.first;
		uint32_t py = position.second;

		uint32_t dx = px > x ? px - x : x - px;
		uint32_t dy = py > y ? py - y : y - py;
		double distance = std::sqrt(dx * dx + dy * dy);

		if (distance <= explosionRadius) {
			penguin->ResetState();
			std::cout << "Penguin at position (" << px << ", " << py << ") was hit by the explosion!" << std::endl;
		}
	}
}



void GameBoard::TriggerExplosion(uint32_t x, uint32_t y) {
	const int explosionRadius = 10;

	// Distrugem celulele din raza exploziei
	for (uint32_t nx = 0; nx < m_rows; ++nx) {
		for (uint32_t ny = 0; ny < m_cols; ++ny) {
			uint32_t dx = nx - x;
			uint32_t dy = ny - y;
			double distance = std::sqrt(dx * dx + dy * dy);

			if (distance <= explosionRadius) {
				m_board[nx][ny] = Cell::Empty;
			}
		}
	}
}


bool GameBoard::IsFarEnoughFromOtherBombs(uint32_t x, uint32_t y)
{
	for (const auto& bomb : m_bombPositions)
	{
		uint32_t dx = bomb.first - x;
		uint32_t dy = bomb.second - y;
		double distance = std::sqrt(dx * dx + dy * dy);

		if (distance < m_minDistanceBombs)
		{
			return false;
		}
	}
	return true;
}

bool GameBoard::WillDestructibleWallAppear()
{
	return std::rand() % 100 < m_destructiblWallChance;
}

bool GameBoard::WillIndestructibleWallAppear()
{
	return std::rand() % 100 < m_indestructiblWallChance;
}

bool GameBoard::WillBombAppear()
{
	return std::rand() % 100 < m_bombChance;
}

bool GameBoard::WillCellBeEmptyAppear() {
	return std::rand() % 100 < m_cellEmptyChance;
}

void GameBoard::CreatePathsBetweenCorners() {
	// Lista colțurilor (pozițiile de start)
	std::vector<std::pair<int, int>> corners = {
		{0, 0}, {0, m_cols - 1}, {m_rows - 1, m_cols - 1}, {m_rows - 1, 0}
	};

	// Conectăm fiecare colț cu următorul (circular)
	for (size_t i = 0; i < corners.size(); ++i) {
		auto start = corners[i];
		auto end = corners[(i + 1) % corners.size()];
		CreatePath(start, end);
	}
}

void GameBoard::CreatePath(Position start, Position end) {
	uint32_t x = start.first; uint32_t y = start.second;
	uint32_t targetX = end.first; uint32_t targetY = end.second;

	// Aleatorizăm direcția deplasărilor
	while (x != targetX || y != targetY) {
		m_board[x][y] = Cell::Empty; // Marcăm celula ca drum liber

		if (std::rand() % 2 == 0) { // Alternăm între mișcările orizontale și verticale
			if (x != targetX) {
				x += (x < targetX) ? 1 : -1;
			}
			else if (y != targetY) {
				y += (y < targetY) ? 1 : -1;
			}
		}
		else {
			if (y != targetY) {
				y += (y < targetY) ? 1 : -1;
			}
			else if (x != targetX) {
				x += (x < targetX) ? 1 : -1;
			}
		}
	}

	m_board[x][y] = Cell::Empty;
}


bool GameBoard::AreCornersConnected() const {
	std::vector<std::vector<bool>> visited(m_rows, std::vector<bool>(m_cols, false));

	// Funcție DFS pentru a explora harta
	std::function<void(int, int)> dfs = [&](int x, int y) {
		if (!IsWithinBounds(x, y) || visited[x][y] || m_board[x][y] != Cell::Empty) return;
		visited[x][y] = true;
		dfs(x + 1, y);
		dfs(x - 1, y);
		dfs(x, y + 1);
		dfs(x, y - 1);
		};

	// Pornim din primul colț
	dfs(0, 0);

	// Verificăm dacă celelalte colțuri sunt vizitate
	return visited[0][0] && visited[0][m_cols - 1] &&
		visited[m_rows - 1][m_cols - 1] && visited[m_rows - 1][0];
}

