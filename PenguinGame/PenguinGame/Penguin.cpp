#include "Penguin.h"

Penguin::Penguin(Position initialPosition,int fireRate):
	m_lives(3),m_points(0),m_score(0),m_position(initialPosition),
		m_isAlive(true),m_weapon(fireRate) {}

