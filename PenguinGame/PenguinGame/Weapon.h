#pragma once
#include "Snowball.h"
#include <chrono> // introd in c++ 11, utila pt gestionare timp

class Weapon
{
public:
	Weapon(int bulletSpeedLevel, int cooldownLevel);

	bool CanShoot() const;
	void ResetTimeSinceLastShot();

	void SetBulletSpeedLevel(int level);
	void SetCooldownLevel(int level);

	float GetBulletSpeed() const;
	int GetCooldown() const;

private:

	int m_bulletSpeedLevel{ 0 }; 
	int m_cooldownLevel{ 0 };     
	float m_bulletSpeed{ 0.25f }; 
	int m_cooldown{ 4000 };       
	std::chrono::steady_clock::time_point m_timeSinceLastShot;

	void UpdateBulletSpeed();
	void UpdateCooldown();
	
};

