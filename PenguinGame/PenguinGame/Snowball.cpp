#include "Snowball.h"

Snowball::Snowball(const std::pair<int, int>& startPosition, Direction launchDirection, float bulletSpeed)
	: m_position{ startPosition },
	m_direction{ launchDirection },
	m_bulletSpeed{ bulletSpeed },
	m_active{ true },
	m_lastUpdate{ std::chrono::steady_clock::now() }
{
}

void Snowball::UpdatePosition() {
	auto currentTime = std::chrono::steady_clock::now();
	std::chrono::duration<float> elapsedTime = currentTime - m_lastUpdate;

	if (elapsedTime.count() >= 1.0f / m_bulletSpeed) {
		m_position = GetNextPosition();
		m_lastUpdate = currentTime;  
	}
}



std::pair<int, int> Snowball::GetNextPosition() const {
	int x = m_position.first;
	int y = m_position.second;

	switch (m_direction) {
	case Direction::Up:    --y; break;
	case Direction::Down:  ++y; break;
	case Direction::Left:  --x; break;
	case Direction::Right: ++x; break;
	}
	return { x, y };
}