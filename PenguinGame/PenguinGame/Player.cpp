#include "Player.h"

Player::Player(const std::string& name, Position initial_position)
	:m_name(name),m_position(initial_position),m_points(0),m_score(0),m_lives(3)

{
}

Player::~Player()
{
}
