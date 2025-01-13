#pragma once
#include <chrono>
#include <deque>
#include "Snowball.h"

class Weapon
{
public:
	Weapon() = default;
	Weapon(int cooldownLevel);

	bool CanShoot() const;
	void ResetTimeSinceLastShot();
	int GetCooldown() const;
	float GetBulletSpeed() const;

	std::deque<Snowball>& GetSnowballs();
	void RemoveInactiveSnowballs();

	void SetCooldownFromLevel(int cooldownLevel);
	void SetBulletSpeedFromLevel(int bulletSpeedLevel);

private:
	int m_cooldown{ 4000 };
	float m_bulletSpeed{ 0.25f };
	std::chrono::steady_clock::time_point m_timeSinceLastShot;
	std::deque<Snowball> m_snowballs;
};

