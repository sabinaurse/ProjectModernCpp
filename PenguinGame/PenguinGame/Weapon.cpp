#include "Weapon.h"
#include <unordered_map>

Weapon::Weapon(int cooldown): m_cooldown(cooldown) {
	m_timeSinceLastShot = std::chrono::steady_clock::now();
}

bool Weapon::CanShoot() const
{
	auto currentTime = std::chrono::steady_clock::now();
	auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - m_timeSinceLastShot).count();
	return elapsed >= m_cooldown;
}

void Weapon::ResetTimeSinceLastShot()
{
	m_timeSinceLastShot = std::chrono::steady_clock::now();
}

int Weapon::GetCooldown() const {
	return m_cooldown;
}

std::vector<Snowball>& Weapon::GetSnowballs()
{
	return m_snowballs;
}

const std::vector<Snowball>& Weapon::GetSnowballs() const
{
	if (m_snowballs.empty()) {
		std::cerr << "Warning: Attempted to access an empty snowball vector." << std::endl;
	}
	return m_snowballs;
}
