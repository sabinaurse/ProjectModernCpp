#include "Player.h"

Player::Player(const std::string& name)
    :m_name{ name }
{
}

Player::Player(const game_database::GamePlayer& dbPlayer)
    : m_name(dbPlayer.name),
    m_points(dbPlayer.points),
    m_score(dbPlayer.score) {}


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

void Player::SetName(const std::string &name)
{
	m_name = name;
}


void Player::AddPoints(int points)
{
    m_points += points;
}

void Player::AddScores(int scores)
{
    m_score += scores;
}

void Player::SetScore(int score)
{
    m_score = score;
}
