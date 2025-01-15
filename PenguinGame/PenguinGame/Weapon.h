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
	float GetBulletSpeed() const;

	std::deque<Snowball>& GetSnowballs();
	void RemoveInactiveSnowballs();

	void SetCooldownFromLevel(int cooldownLevel);

	template <typename T>
	void SetBulletSpeedFromLevel(T bulletSpeedLevel);

private:
	int m_cooldown;
	float m_bulletSpeed;
	std::chrono::steady_clock::time_point m_timeSinceLastShot;
	std::deque<Snowball> m_snowballs;
};


template <typename T>
void Weapon::SetBulletSpeedFromLevel(T bulletSpeedLevel)
{
	if (bulletSpeedLevel == 1)
	{
		m_bulletSpeed = 3.5f;
	}
	else
	{
		m_bulletSpeed = 3.25f;
	}
}

