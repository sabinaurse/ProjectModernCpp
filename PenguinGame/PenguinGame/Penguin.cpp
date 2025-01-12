#include "Penguin.h"
#include "Snowball.h"

#define RADIUS_OF_COLLISION 15.0f
#define MOVE_STEP 1

Penguin::Penguin(Player* player, Position initialPosition, const Weapon& weapon)
	: m_player{ player }, m_initialPosition{ initialPosition },
	m_weapon{ weapon }
{ }

void Penguin::Move(char direction, const MapGen::GameBoard& gameBoard) {
	if (!m_isAlive) return;

	Position delta{ 0, 0 };
	switch (direction) {
	case 'W': delta.second = -MOVE_STEP; break; // Sus
	case 'S': delta.second = MOVE_STEP; break;  // Jos
	case 'A': delta.first = -MOVE_STEP; break;  // Stânga
	case 'D': delta.first = MOVE_STEP; break;   // Dreapta
	default:
		std::cout << "Invalid direction. Use W, A, S, D for movement." << std::endl;
		return;
	}

	Position newPosition = { m_position.first + delta.first, m_position.second + delta.second };

	// Verificăm limitele hărții
	if (newPosition.second >= 0 && newPosition.second < gameBoard.GetRows() &&
		newPosition.first >= 0 && newPosition.first < gameBoard.GetCols()) {

		// Obținem tipul celulei țintă
		const auto& board = gameBoard.GetBoard();
		int cellType = board[newPosition.second][newPosition.first]; // Observăm că matricea este accesată invers (rând, coloană)

		// Permitem mutarea doar pe celule de tip `Empty` (0)
		if (cellType == 0) {
			m_position = newPosition;
			m_currentDirection = direction;
			std::cout << "Penguin moved to (" << newPosition.first << ", " << newPosition.second << ")." << std::endl;
		}
		else {
			std::cout << "Move blocked by cell type: " << cellType << " at ("
				<< newPosition.first << ", " << newPosition.second << ")." << std::endl;
		}
	}
	else {
		std::cout << "Move blocked! Out of bounds at (" << newPosition.first << ", " << newPosition.second << ")." << std::endl;
	}
}

void Penguin::Fire() {
	if (!m_isAlive) return;

	if (m_weapon.CanShoot()) {
		Direction direction;

		switch (m_currentDirection) {
		case 'W': direction = Direction::Up; break;
		case 'S': direction = Direction::Down; break;
		case 'A': direction = Direction::Left; break;
		case 'D': direction = Direction::Right; break;
		default:
			std::cout << "Invalid direction for firing!" << std::endl;
			return;
		}

		Snowball newSnowball(m_position, direction);
		m_weapon.GetSnowballs().emplace_back(newSnowball);
		m_weapon.ResetTimeSinceLastShot();
	}
	else {
		std::cout << "Penguin can't shoot yet. Waiting for reload." << std::endl;
	}
}

void Penguin::EliminateEnemy()
{
	m_enemiesEliminated++;
	m_player->AddPoints(100);  
	std::cout << "Player " << m_player->GetName() << " eliminated an enemy! Total points: " << m_player->GetPoints() << std::endl;
}

void Penguin::TakeDamage()
{
	if (!m_isAlive) return;
	if (--m_lives <= 0)
	{
		m_isAlive = false;

		static int eliminationCount = 0;
		MarkAsEliminated(++eliminationCount);

		std::cout << "Penguin has been defeated!" << std::endl;
	}
}

void Penguin::ResetState()
{
	m_position = m_initialPosition;
	m_isAlive = true;
	m_weapon.GetSnowballs().clear();
	std::cout << "Penguin reset to initial position (" << m_initialPosition.first << ", " << m_initialPosition.second << ")." << std::endl;
}

void Penguin::MarkAsEliminated(int eliminationOrder) {
	m_eliminationOrder = eliminationOrder;
}

int Penguin::GetEliminationOrder() const {
	return m_eliminationOrder;
}
