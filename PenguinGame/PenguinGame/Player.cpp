#include "Player.h"

Player::Player(const std::string& name, const std::string& password)
	:m_name(name),m_password(password), m_points(0), m_score(0), m_weaponBoostCount(0), m_speedBoostCount(0)
{
}

std::string Player::GetName() const
{
	return m_name;
}

int Player::GetPoints() const
{
	return m_points;
}

int Player::GetScore() const
{
	return m_score;
}

void Player::checkWeaponBoost() 
{
	if (m_points >= 500 && m_weaponBoostCount < 4) {
		m_weaponBoostCount++;
		std::cout << "Weapon fire rate improved!" << std::endl;
	}
}

void Player::checkSpeedBoost() 
{
	if (m_score >= 10 && m_speedBoostCount < 1) {
		m_speedBoostCount++;
		std::cout << "Bullet speed doubled!" << std::endl;
	}
}

void Player::addPoints(int points) {
	m_points += points;
	checkWeaponBoost();
}

void Player::addScore(int score) {
	m_score += score;
	checkSpeedBoost();
}
