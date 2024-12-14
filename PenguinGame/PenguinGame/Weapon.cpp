#include "Weapon.h"
#include "Snowball.h"
#include <unordered_map>

Weapon::Weapon(int bulletSpeedLevel, int cooldownLevel)
	: m_bulletSpeedLevel(bulletSpeedLevel), m_cooldownLevel(cooldownLevel) {
	UpdateBulletSpeed();
	UpdateCooldown();
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


void Weapon::UpdateBulletSpeed() {
	static const std::unordered_map<int, float> speedMap = {
		{0, 0.25f}, {1, 0.5f}, {2, 1.0f}, {3, 2.0f}
	};
	m_bulletSpeed = speedMap.at(m_bulletSpeedLevel);
}

void Weapon::UpdateCooldown() {
	static const std::unordered_map<int, int> cooldownMap = {
		{0, 4000}, {1, 3000}, {2, 2000}, {3, 1000}
	};
	m_cooldown = cooldownMap.at(m_cooldownLevel);
}

void Weapon::SetBulletSpeedLevel(int level) {
	m_bulletSpeedLevel = level;
	UpdateBulletSpeed();
}

void Weapon::SetCooldownLevel(int level) {
	m_cooldownLevel = level;
	UpdateCooldown();
}

float Weapon::GetBulletSpeed() const {
	return m_bulletSpeed;
}

int Weapon::GetCooldown() const {
	return m_cooldown;
}