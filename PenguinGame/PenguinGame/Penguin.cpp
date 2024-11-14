#include "Penguin.h"

Penguin::Penguin(std::pair<int,int> initialPosition,int fireRate):
	m_lives(3), m_position(initialPosition), m_isAlive(true), m_weapon(fireRate) {}

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

			m_snowballs.emplace_back(std::make_tuple(m_position.first, m_position.second), directionString);
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
	// alte actualizari eventual
}

void Penguin::CheckForWeaponUpgrade() {
	if (m_score >= 10) {
		for (auto& snowball : m_snowballs) {
			snowball.DoubleSpeed(); // Aplicăm dublarea vitezei pe fiecare bulgăre}
		}
	}
	m_score -=10; // se reseteaza/ se scade sau ramane in continuare?
	std::cout << "Penguin's snowball speed doubled!" << std::endl;

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

void Penguin::ResetCharacter() {
	m_lives = 3;
	m_position = { 0,0 };
	m_isAlive = true;
	std::cout << "Penguin has been reset" << std::endl;
}

void Penguin::Move(int dx, int dy)
{
	if (!m_isAlive) return;
	//Actualizam pozitia de baza dx si dy (intelegem dx, dy ca deplasari pe axele x si y)
	m_position.first += dx;
	m_position.second += dy;
	std::cout << "Penguin moved to position (" << m_position.first << ", " << m_position.second << ")." << std::endl;
}

void Penguin::DoubleSnowballSpeed()
{
	for (auto& snowball : m_snowballs) {
		snowball.DoubleSpeed();
	}
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