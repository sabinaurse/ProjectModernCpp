#include "Player.h"

Player::Player(const std::string& name)
    :m_name{ name }
{
}

Player::Player(const game_database::GamePlayer& dbPlayer)
    : m_name(dbPlayer.name),
    m_points(dbPlayer.points),
    m_score(dbPlayer.score),
    m_bulletSpeedLevel(dbPlayer.bullet_speed_level),
    m_cooldownLevel(dbPlayer.cooldown_level) {}


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

int Player::GetBulletSpeedLevel() const {
    return m_bulletSpeedLevel;
}

void Player::SetBulletSpeedLevel(int level) {
    m_bulletSpeedLevel = level;
}

int Player::GetCooldownLevel() const {
    return m_cooldownLevel;
}

void Player::SetCooldownLevel(int level) {
    m_cooldownLevel = level;
}