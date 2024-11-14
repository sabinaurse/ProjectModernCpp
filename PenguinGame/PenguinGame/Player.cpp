#include "Player.h"

Player::Player(const std::string& name, const std::string& password)
	:m_name(name), m_password(password), m_points(0), m_score(0)
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

void Player::SetName(std::string name)
{
	m_name = name;
}

