#include "GameBoard.h"
#include "Penguin.h"
#include <stdexcept>

GameBoard::GameBoard(int rows, int cols, int bombChance, int destructiblWallChance, int indestructiblWallChance,int emptyCellChance, int maxBombs, int minDistanceBombs)
	: m_rows{ rows }, m_cols{ cols },
	m_bombChance{ bombChance }, m_destructiblWallChance{ destructiblWallChance },
	m_indestructiblWallChance{ indestructiblWallChance },m_cellEmptyChange(emptyCellChance), m_maxBombs{ maxBombs },
	m_minDistanceBombs{ minDistanceBombs }
{
	InitializeBoard();
}


void GameBoard::InitializeBoard()
{
	m_board.clear();
	m_board.resize(m_rows, std::vector<Cell>(m_cols, Cell::Indestructible_Wall));
	m_bombPositions.clear();
	m_bombsPlaced = 0;

	// Creăm drumuri libere între colțuri
	CreatePathsBetweenCorners();

	for (int i = 0; i < m_rows; ++i)
		for (int j = 0; j < m_cols; ++j)
		{
			// Evităm să suprascriem drumurile libere
			if (m_board[i][j] == Cell::Empty) {
				continue;
			}

			if (willCellBeEmptyAppear()) {
				m_board[i][j] = Cell::Empty;
			}
			else if (willDestructibleWallAppear())
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
	// Verificăm dacă colțurile sunt conectate
	if (!AreCornersConnected()) {
		throw std::runtime_error("The generated map does not have connected corners!");
	}
}

void GameBoard::PrintBoard()
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


int GameBoard::GetRows() const
{
	return m_rows;
}

int GameBoard::GetCols() const
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


bool GameBoard::IsWithinBounds(int x, int y) const
{
	return x >= 0 && x < m_rows && y >= 0 && y < m_cols;
}

Cell GameBoard::GetCell(int x, int y) const
{
	if (IsWithinBounds(x, y))
		return m_board[x][y];

	throw std::out_of_range("Coordonatele sunt în afara limitelor tabelei de joc");
}

void GameBoard::SetCell(int x, int y, Cell cellType)
{
	if (IsWithinBounds(x, y))
		m_board[x][y] = cellType;
	else
		throw std::out_of_range("Coordonatele sunt în afara limitelor tabelei de joc");
}


void GameBoard::DestroyCell(int x, int y) {
	if (IsWithinBounds(x, y)) {
		m_board[x][y] = Cell::Empty; // Sau altă logică de distrugere
	}
}

void GameBoard::DestroyCell(int x, int y, std::vector<Penguin*>& penguins) {
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

std::vector<std::pair<int, int>> GameBoard::GetStartingPositions() {
	std::vector<std::pair<int, int>> startingPositions = {
		{0, 0}, {0, m_cols - 1}, {m_rows - 1, m_cols - 1}, {m_rows - 1, 0}
	};

	for (const auto& pos : startingPositions) {
		if (!IsWithinBounds(pos.first, pos.second) || GetCell(pos.first, pos.second) != Cell::Empty) {
			throw std::runtime_error("Invalid starting position found!");
		}
	}

	return startingPositions;
}

void GameBoard::DetonateBomb(int x, int y) {
	const int explosionRadius = 10;

	std::vector<Penguin*> emptyPenguins;

	for (int i = 0; i < m_rows; ++i) {
		for (int j = 0; j < m_cols; ++j) {
			int dx = i - x;
			int dy = j - y;
			double distance = std::sqrt(dx * dx + dy * dy);

			if (distance <= explosionRadius) {
				DestroyCell(i, j, emptyPenguins);
			}
		}
	}
}

void GameBoard::TriggerExplosion(int x, int y, std::vector<Penguin*>& penguins)
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
			penguin->ResetState(); // ii luam toate cele 3 vieti pt a-l reseta
			std::cout << "Penguin at position (" << px << ", " << py << ") was hit by the explosion!" << std::endl;
		}
	}
}

void GameBoard::TriggerExplosion(int x, int y) {
	const int explosionRadius = 10;

	// Distrugem celulele din raza exploziei
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

bool GameBoard::willCellBeEmptyAppear() {
	return std::rand() % 100 < m_cellEmptyChange;
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

void GameBoard::CreatePath(std::pair<int, int> start, std::pair<int, int> end) {
	int x = start.first, y = start.second;
	int targetX = end.first, targetY = end.second;

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



