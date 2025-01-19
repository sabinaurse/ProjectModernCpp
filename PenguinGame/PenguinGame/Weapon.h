#pragma once
#include <chrono>
#include <deque>
#include "Snowball.h"

class Weapon
{
public:
	Weapon() = default;
	Weapon(int cooldownLevel, int bulletSpeedLevel);

	bool CanShoot() const;
	void ResetTimeSinceLastShot();
	int GetCooldown() const;
	
	template <typename T>
	T GetBulletSpeed() const;

	std::deque<Snowball>& GetSnowballs();
	void RemoveInactiveSnowballs();

	void SetCooldownFromLevel(int cooldownLevel);
	void SetBulletSpeedFromLevel(int bulletSpeedLevel);

private:
	int m_cooldown;
	float m_bulletSpeed;
	std::chrono::steady_clock::time_point m_timeSinceLastShot;
	std::deque<Snowball> m_snowballs;
};

template <typename T>
T Weapon::GetBulletSpeed() const
{
	return static_cast<T>(m_bulletSpeed);
}