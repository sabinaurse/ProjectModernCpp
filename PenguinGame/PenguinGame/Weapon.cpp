#include "Weapon.h"

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

std::deque<Snowball>& Weapon::GetSnowballs()
{
	return m_snowballs;
}

void Weapon::RemoveInactiveSnowballs() {
	m_snowballs.erase(
		std::remove_if(
			m_snowballs.begin(), m_snowballs.end(),
			[](const Snowball& snowball) { return !snowball.IsActive(); }
		),
		m_snowballs.end()
	);
}
