#include "Penguin.h"
#include "Snowball.h"

#define MOVE_STEP 1

Penguin::Penguin(Player* player, Position initialPosition, const Weapon& weapon)
	: m_player{ player }, m_initialPosition{ initialPosition },
	m_weapon{ weapon }
{ }

void Penguin::Move(char direction, const MapGen::GameBoard& gameBoard) {
	if (!m_isAlive) return;

	Position delta{ 0, 0 };

	switch (direction) {
	case 'W': delta.first = -MOVE_STEP; break;
	case 'S': delta.first = MOVE_STEP; break;
	case 'A': delta.second = -MOVE_STEP; break;
	case 'D': delta.second = MOVE_STEP; break;
	default:
		std::cout << "Invalid direction. Use W, A, S, D for movement." << std::endl;
		return;
	}

	Position newPosition = { m_position.first + delta.first, m_position.second + delta.second };

	if (newPosition.first >= 0 && newPosition.first < gameBoard.GetRows() &&
		newPosition.second >= 0 && newPosition.second < gameBoard.GetCols()) {

		const auto& board = gameBoard.GetBoard();
		int cellType = board[newPosition.first][newPosition.second];

		if (cellType == 0) {
			m_position = newPosition;
			m_currentDirection = direction;
			std::cout << "Penguin direction set to: " << m_currentDirection << std::endl;
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

		float bulletSpeed = m_weapon.GetBulletSpeed();

		Snowball newSnowball(m_position, direction, bulletSpeed);
		m_weapon.GetSnowballs().emplace_back(newSnowball);

		std::cout << "Snowball created at (" << newSnowball.GetPosition().first << ", "
			<< newSnowball.GetPosition().second << ") in direction " << direction
			<< " with speed " << newSnowball.GetBulletSpeed() << std::endl;

		m_weapon.ResetTimeSinceLastShot();
	}
	else {
		std::cout << "Penguin can't shoot yet. Waiting for reload." << std::endl;
	}
}

void Penguin::EliminateEnemy()
{
	if (!m_player) return;
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
