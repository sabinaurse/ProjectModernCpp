#include "Penguin.h"
#include "Snowball.h"

#define RADIUS_OF_COLLISION 15.0f

Penguin::Penguin(Player* player, std::pair<int, int> initialPosition, int fireRate) :
	m_player(player),m_initialPosition(initialPosition), m_position(initialPosition),m_points(0),m_score(0), m_enemiesEliminated(0),
	m_isAlive(true), m_weapon(fireRate), m_lives(3), m_speedBoostCount(0), m_weaponBoostCount(0),m_resetCount(0) {}

void Penguin::Fire(int mouseX, int mouseY, bool isMouseControlled, char keyboardDirection)
{
	if (!m_isAlive) return; // verificam daca personajul e mort
	// Verificam daca arma poate trage
	if (m_weapon.CanShoot()) {
		auto direction = FireDirectionProjectile(mouseX, mouseY, isMouseControlled, keyboardDirection);// calc directia de tragere

		if (direction.first != 0.0f || direction.second != 0.0f) {
			std::string directionString; // string pt directia de tragere
			if (direction.first > 0) directionString = "right";
			else if (direction.first < 0) directionString = "left";
			else if (direction.second > 0) directionString = "down";
			else if (direction.second < 0) directionString = "up";

			m_snowballs.emplace_back(std::make_pair(m_position.first, m_position.second), directionString);
			std::cout << "Penguin fired a snowball from position (" << m_position.first << ", " << m_position.second << ")." << std::endl;
		}
		m_weapon.ResetTimeSinceLastShot();
	}
	else {
		std::cout << "Penguin can't shoot yet. Waiting for reload." << std::endl;
	}
}

void Penguin::UpdateWeapon() {
	CheckForWeaponUpgrade();
}

void Penguin::CheckForWeaponUpgrade() {
	if (m_score >= 10 && m_speedBoostCount < 1)
	{
		m_speedBoostCount++;
		for (auto& snowball : m_snowballs) {
			snowball.DoubleSpeed(); // Aplicam dublarea vitezei pe fiecare bulgare
		}
		std::cout << "Bullet speed doubled!" << std::endl;
	}
	std::cout << "Penguin's snowball speed doubled!" << std::endl;
}

void Penguin::UpgradeFireRate() {
	if (m_points >= 500 && m_weaponBoostCount < 4) {
		m_points -= 500;
		m_weaponBoostCount++;

		int newFireRate = m_weapon.GetFireRate() / 2;
		m_weapon.SetFireRate(newFireRate);

		std::cout << "Weapon fire rate improved! New fire rate: " << newFireRate << " ms" << std::endl;
	}
	else {
		std::cout << "Not enough points or max upgrades reached." << std::endl;
	}
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


void Penguin::EliminateEnemy()
{
	m_enemiesEliminated++;  // Incrementam numarul de inamici eliminati
	m_score += 100;  // Adugam 100 de puncte pentru fiecare inamic eliminat
	std::cout << "Penguin eliminated an enemy! Total enemies eliminated: " << m_enemiesEliminated << std::endl;
}

void Penguin::TakeDamage(int damage)
{
	if (!m_isAlive) return;

	m_lives -= damage;
	std::cout << "Penguin took " << damage << " damage. Lives remaining: " << m_lives << std::endl;

	if (m_lives <= 0) {
		m_isAlive = false;
		std::cout << "Penguin has been defeated and is reset to initial position!" << std::endl;
		ResetCharacter();
	}
}

void Penguin::ResetState()
{
	m_lives = 3;
	m_position = m_initialPosition; 
	m_isAlive = true;

	std::cout << "Penguin reset to initial position (" << m_initialPosition.first<< ", " << m_initialPosition.second << ")." << std::endl;
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
	std::cout << "Penguin has been reset. Reset count: " << m_resetCount<< " / " << m_maxResets << "." << std::endl;
}

void Penguin::Move(int dx, int dy)
{
	if (!m_isAlive) return;
	//Actualizam pozitia de baza dx si dy (intelegem dx, dy ca deplasari pe axele x si y)
	m_position.first += dx;
	m_position.second += dy;
	std::cout << "Penguin moved to position (" << m_position.first << ", " << m_position.second << ")." << std::endl;
}


std::pair<float, float> Penguin::FireDirectionProjectile(int mouseX, int mouseY, bool isMouseControlled, char keyboardDirection)
{
	float dx = 0;
	float dy = 0;

	if (isMouseControlled)
	{
		//Calculam directia pe baza pozitiei mouse-ului
		dx = static_cast<float>(mouseX - m_position.first);
		dy = static_cast<float>(mouseY - m_position.second);

		//Normalizam vectorul directiei
		double length = std::sqrt(dx * dx + dy * dy);
		if (length != 0)
		{
			dx /= static_cast<float>(length);
			dy /= static_cast<float>(length);
		}
	}
	else {
		//Setam directia pe baza tastaturii
		switch (keyboardDirection) {
		case 'W': dy = -1; break;
		case 'S': dy = 1; break;
		case 'A': dx = -1; break;
		case 'D': dx = 1; break;
		default: break;
		}
	}

	return std::make_pair(dx, dy);
}