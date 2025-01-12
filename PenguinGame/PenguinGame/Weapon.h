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

	std::deque<Snowball>& GetSnowballs();
	void RemoveInactiveSnowballs();

private:
	int m_cooldown{ 4000 };       
	std::chrono::steady_clock::time_point m_timeSinceLastShot;
	std::deque<Snowball> m_snowballs;
};

