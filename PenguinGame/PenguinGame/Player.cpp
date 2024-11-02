#include "Player.h"

Player::Player(const std::string& name)
	:m_name(name),m_points(0),m_score(0)

{
}

std::string Player::getName()
{
	return m_name;
}

int Player::getPoints()
{
	return m_points;
}

int Player::getScore()
{
	return m_score;
}

Player::~Player()
{
}
