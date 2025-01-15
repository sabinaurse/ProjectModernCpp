#include "Weapon.h"

Weapon::Weapon(int cooldownLevel, int bulletSpeedLevel)
    : m_timeSinceLastShot(std::chrono::steady_clock::now())
{
    SetCooldownFromLevel(cooldownLevel);
    SetBulletSpeedFromLevel(bulletSpeedLevel);
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

float Weapon::GetBulletSpeed() const
{
    return m_bulletSpeed;
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

void Weapon::SetCooldownFromLevel(int cooldownLevel)
{
    switch (cooldownLevel)
    {
    case 0:
        m_cooldown = 4000;
        break;
    case 1:
        m_cooldown = 2000;
        break;
    case 2:
        m_cooldown = 1000;
        break;
    case 3:
        m_cooldown = 500;
        break;
    case 4:
        m_cooldown = 250;
        break;
    default:
        m_cooldown = 4000;
        break;
    }
}
