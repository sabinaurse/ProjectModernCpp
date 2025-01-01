#include "Penguin.h"
#include "Snowball.h"

#define RADIUS_OF_COLLISION 15.0f
#define MOVE_STEP 1

Penguin::Penguin(Player* player, Position initialPosition, int fireRate)
	: m_player{ player }, m_initialPosition{ initialPosition }, m_position{ initialPosition },
	m_weapon(player->GetBulletSpeedLevel(), player->GetCooldownLevel()) // Setăm direct nivelurile
{
	// Sincronizăm viteza glonțului cu nivelul jucătorului
	m_bulletSpeed = m_weapon.GetBulletSpeed();
}


void Penguin::Move(char direction,const MapGen::GameBoard& gameBoard) {
	if (!m_isAlive) return; // Nu poate să se miște dacă e mort.

	Position delta{ 0, 0 };

	switch (direction) {
	case 'W': delta.second = -MOVE_STEP; break; // Sus
	case 'S': delta.second = MOVE_STEP; break;  // Jos
	case 'A': delta.first = -MOVE_STEP; break; // Stânga
	case 'D': delta.first = MOVE_STEP; break;  // Dreapta
	default:
		std::cout << "Invalid direction. Use W, A, S, D for movement." << std::endl;
		return;
	}

	Position newPosition = { m_position.first + delta.first, m_position.second + delta.second };
	m_position = newPosition; 

	// Verificăm dacă poziția este validă și liberă.
	/*if (board.IsWithinBounds(newPosition.first, newPosition.second) &&
		board.GetCell(newPosition.first, newPosition.second) == Cell::Empty) {
		m_position = newPosition; // Actualizăm poziția.
		m_currentDirection = direction; // Actualizăm direcția curentă.
		std::cout << "Penguin moved to (" << newPosition.first << ", " << newPosition.second
			<< ") facing " << m_currentDirection << "." << std::endl;
	}
	else {
		std::cout << "Move blocked!" << std::endl;
	}*/
}

void Penguin::Fire() {
	if (!m_isAlive) return;

	if (m_weapon.CanShoot()) {
		std::string direction;

		// Convertim direcția curentă într-un string corespunzător.
		switch (m_currentDirection) {
		case 'W': direction = "up"; break;    // Sus
		case 'S': direction = "down"; break; // Jos
		case 'A': direction = "left"; break; // Stânga
		case 'D': direction = "right"; break; // Dreapta
		default:
			std::cout << "Invalid direction for firing!" << std::endl;
			return;
		}

		// Creăm glonțul folosind constructorul Snowball.
		m_snowballs.emplace_back(m_position, direction, m_bulletSpeed);
		std::cout << "Penguin fired a snowball with speed " << m_bulletSpeed << " m/s in direction " << direction << "." << std::endl;

		m_weapon.ResetTimeSinceLastShot();
	}
	else {
		std::cout << "Penguin can't shoot yet. Waiting for reload." << std::endl;
	}
}

void Penguin::UpgradeBulletSpeed() {
	int currentLevel = m_player->GetBulletSpeedLevel();
	if (currentLevel < 3) { 
		m_player->SetBulletSpeedLevel(currentLevel + 1);
		m_weapon.SetBulletSpeedLevel(currentLevel + 1);
		m_bulletSpeed = m_weapon.GetBulletSpeed();
		std::cout << "Bullet speed upgraded to level " << currentLevel + 1
			<< " with speed " << m_bulletSpeed << " m/s." << std::endl;
	}
	else {
		std::cout << "Bullet speed is already at the maximum level!" << std::endl;
	}
}


void Penguin::CheckBulletSpeedUpgrade() {
	if (m_player->GetScore() >= 10 && !m_speedBoostApplied) {
		UpgradeBulletSpeed();
		m_speedBoostApplied = true;
	}
}

void Penguin::EliminateEnemy()
{
	m_enemiesEliminated++;
	m_player->AddPoints(100);  // Adugam 100 de puncte pentru fiecare inamic eliminat
	std::cout << "Player " << m_player->GetName() << " eliminated an enemy! Total points: " << m_player->GetPoints() << std::endl;
}

void Penguin::UpgradeFireRate() {
	int currentLevel = m_player->GetCooldownLevel();
	if (currentLevel < 3) {
		m_player->SetCooldownLevel(currentLevel + 1);
		m_weapon.SetCooldownLevel(currentLevel + 1);
		std::cout << "Cooldown upgraded to level " << currentLevel + 1
			<< " with fire rate " << m_weapon.GetCooldown() << " ms." << std::endl;
	}
	else {
		std::cout << "Cooldown is already at the maximum level!" << std::endl;
	}
}

void Penguin::TakeDamage()
{
	if (!m_isAlive) return;
	if (--m_lives <= 0) 
	{
		m_isAlive = false;

		// Marchează pinguinul ca eliminat
		static int eliminationCount = 0;
		MarkAsEliminated(++eliminationCount);

		std::cout << "Penguin has been defeated!" << std::endl;
	}
}

void Penguin::ResetState()
{
	m_position = m_initialPosition;
	m_isAlive = true;
	m_snowballs.clear();
	std::cout << "Penguin reset to initial position (" << m_initialPosition.first << ", " << m_initialPosition.second << ")." << std::endl;
}

float Penguin::GetBulletSpeed() const {
	return m_bulletSpeed;
}

std::vector<Snowball>& Penguin::GetSnowballs() {
	return m_snowballs;
}

const std::vector<Snowball>& Penguin::GetSnowballs() const {
	return m_snowballs;
}

void Penguin::RemoveInactiveSnowballs() {
	m_snowballs.erase(
		std::remove_if(m_snowballs.begin(), m_snowballs.end(),
			[](const Snowball& snowball) { return !snowball.IsActive(); }),
		m_snowballs.end()
	);
}

void Penguin::MarkAsEliminated(int eliminationOrder) {
	m_eliminationOrder = eliminationOrder;
}

int Penguin::GetEliminationOrder() const {
	return m_eliminationOrder;
}
