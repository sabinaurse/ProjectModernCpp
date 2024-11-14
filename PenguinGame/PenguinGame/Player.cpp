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


void Player::AddPoints(int points)
{
    m_points += points;
}

void Player::SetScore(int score)
{
    m_score = score;
}

void Player::UpdateScore()
{
    if (m_score == 0) {
        std::cout << m_name << " has no score yet." << std::endl;
    }
    else {
        std::cout << m_name << " scored: " << m_score << std::endl;
    }
}