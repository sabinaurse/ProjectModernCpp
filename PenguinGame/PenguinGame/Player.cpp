#include "Player.h"

//Player::Player(const std::string& name)
//    :m_name{ name }
//{
//}

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

int Player::GetBulletSpeedLevel() const {
    return m_bulletSpeedLevel;
}

int Player::GetCooldownLevel() const {
    return m_cooldownLevel;
}

void Player::SetName(std::string&& name) {
    m_name = std::move(name);
}

void Player::SetPoints(uint32_t points)
{
    m_points = points;
}

void Player::SetScore(uint32_t score)
{
    m_score = score;
}

void Player::SetBulletSpeedLevel(uint8_t level) {
    m_bulletSpeedLevel = level;
}


void Player::SetCooldownLevel(uint8_t level) {
    m_cooldownLevel = level;
}

void Player::AddPoints(uint32_t points)
{
    m_points += points;
}

void Player::AddScores(uint32_t scores)
{
    m_score += scores;
}


Player::Player(const std::string& name, uint32_t score)
    : m_name{ name }, m_score{ score } {
}

