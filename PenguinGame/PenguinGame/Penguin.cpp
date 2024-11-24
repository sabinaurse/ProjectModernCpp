#include "Penguin.h"
#include "Snowball.h"

#define RADIUS_OF_COLLISION 15.0f

Penguin::Penguin(Player* player,Position initialPosition, int fireRate) :
	m_player{ player }, m_initialPosition{ initialPosition }, m_position{ initialPosition },
	 m_weapon(fireRate) {}

void Penguin::Move(char direction, const GameBoard& board) {
	if (!m_isAlive) return; // Nu poate să se miște dacă e mort.

	Position delta{ 0, 0 };

	switch (direction) {
	case 'W': delta.second = -1; break; // Sus
	case 'S': delta.second = 1; break;  // Jos
	case 'A': delta.first = -1; break; // Stânga
	case 'D': delta.first = 1; break;  // Dreapta
	default:
		std::cout << "Invalid direction. Use W, A, S, D for movement." << std::endl;
		return;
	}

	Position newPosition = { m_position.first + delta.first, m_position.second + delta.second };

	// Verificăm dacă poziția este validă și liberă.
	if (board.isWithinBounds(newPosition.first, newPosition.second) &&
		board.getCell(newPosition.first, newPosition.second) == Cell::Empty) {
		m_position = newPosition; // Actualizăm poziția.
		m_currentDirection = direction; // Actualizăm direcția curentă.
		std::cout << "Penguin moved to (" << newPosition.first << ", " << newPosition.second
			<< ") facing " << m_currentDirection << "." << std::endl;
	}
	else {
		std::cout << "Move blocked!" << std::endl;
	}
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
	if (!m_speedBoostApplied) {
		m_bulletSpeed *= 2; // Dublăm viteza.
		m_speedBoostApplied = true;
		std::cout << "Bullet speed upgraded to " << m_bulletSpeed << " m/s." << std::endl;
	}
	else {
		std::cout << "Bullet speed upgrade already applied." << std::endl;
	}
}

void Penguin::CheckBulletSpeedUpgrade() {
	if (m_score >= 10 && !m_speedBoostApplied) {
		UpgradeBulletSpeed();
		m_speedBoostApplied = true;
	}
}

void Penguin::EliminateEnemy()
{
	m_enemiesEliminated++;
	m_points += 100;  // Adugam 100 de puncte pentru fiecare inamic eliminat
	std::cout << "Penguin eliminated an enemy! Total enemies eliminated: " << m_enemiesEliminated << std::endl;
}

void Penguin::UpgradeFireRate() {
	if (m_points >= 500 && m_weaponBoostCount < 4) {
		m_points -= 500;
		m_weaponBoostCount++;

		m_weapon.UpgradeFireRate();

		std::cout << "Weapon fire rate improved!"<< std::endl;
	}
	else {
		std::cout << "Not enough points or max upgrades reached." << std::endl;
	}
}

void Penguin::TakeDamage()
{
	if (!m_isAlive) return;
	if (--m_lives <= 0) { // Scădem o viață și verificăm dacă mai are vieți rămase.
		m_isAlive = false;
		std::cout << "Penguin has been defeated!" << std::endl;
	}
}

void Penguin::ResetState()
{
	m_lives = 3;
	m_position = m_initialPosition;
	m_isAlive = true;
	m_snowballs.clear();
	std::cout << "Penguin reset to initial position (" << m_initialPosition.first << ", " << m_initialPosition.second << ")." << std::endl;
}

void Penguin::ResetCharacter() {
	m_resetCount++;

	if (m_resetCount >= m_maxResets)
	{
		m_isAlive = false;
		std::cout << "Penguin has been permanently eliminated after " << m_resetCount << " resets" << std::endl;
		return;
	}

	ResetState();
	std::cout << "Penguin has been reset. Reset count: " << m_resetCount << " / " << m_maxResets << "." << std::endl;
}

bool Penguin::CollidesWith(Penguin* otherPenguin, GameBoard& gameBoard, const std::vector<Snowball>& snowballs) {
	// Verificam coliziunile cu alti pinguini
	if (otherPenguin && otherPenguin->IsAlive()) {
		auto myPosition = GetPosition();
		auto otherPosition = otherPenguin->GetPosition();

		int distanceX = myPosition.first - otherPosition.first;
		int distanceY = myPosition.second - otherPosition.second;
		float distance = std::sqrt(distanceX * distanceX + distanceY * distanceY);

		if (distance < RADIUS_OF_COLLISION) {
			std::cout << "Penguin collision detected." << std::endl;
			return true;
		}
	}

	// Verificam coliziunile cu bulgarii de zapada
	for (const auto& snowball : snowballs) {
		if (snowball.IsActive()) {
			auto snowballPosition = snowball.GetPosition();
			int distanceX = GetPosition().first - snowballPosition.first;
			int distanceY = GetPosition().second - snowballPosition.second;
			float distance = std::sqrt(distanceX * distanceX + distanceY * distanceY);

			if (distance < RADIUS_OF_COLLISION) {
				std::cout << "Snowball collision detected." << std::endl;
				return true;
			}
		}
	}

	// Verificam coliziunile cu elementele de pe harta
	auto myPosition = GetPosition();
	int px = myPosition.first;
	int py = myPosition.second;

	if (gameBoard.isWithinBounds(px, py)) {
		Cell cell = gameBoard.getCell(px, py);

		switch (cell) {
		case Cell::Destructible_Wall:
			std::cout << "Collision with destructible wall at (" << px << ", " << py << ")." << std::endl;
			return true;
		case Cell::Hidden_Bomb: {
			std::vector<Penguin*> affectedPenguins = { this };
			gameBoard.triggerExplosion(px, py, affectedPenguins);
			return true;
		}
		case Cell::Indestructible_Wall:
			std::cout << "Collision with indestructible wall at (" << px << ", " << py << ")." << std::endl;
			return true;
		default:
			break;
		}
	}

	return false; // Nicio coliziune detectata
}

float Penguin::GetBulletSpeed() const {
	return m_bulletSpeed;
}
