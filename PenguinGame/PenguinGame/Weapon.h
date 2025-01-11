#pragma once
#include "Snowball.h"
#include <chrono>
#include <vector>

class Weapon
{
public:
	Weapon() = default;
	Weapon(int cooldownLevel);

	bool CanShoot() const;
	void ResetTimeSinceLastShot();

	int GetCooldown() const;
	// DE IMPLEMENTAT
	std::vector<Snowball>& GetSnowballs();
	const std::vector<Snowball>& GetSnowballs() const;

private:
	int m_cooldown{ 4000 };       
	std::chrono::steady_clock::time_point m_timeSinceLastShot;

	std::vector<Snowball> m_snowballs;
};

