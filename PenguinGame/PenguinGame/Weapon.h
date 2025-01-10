#pragma once
#include "Snowball.h"
#include <chrono> 

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

	int m_bulletSpeedLevel{ 0 };  //-> de sters
	int m_cooldownLevel{ 0 };     //-> de sters
	float m_bulletSpeed{ 0.25f }; //-> de sters/mutat in snowball
	int m_cooldown{ 4000 };       
	std::chrono::steady_clock::time_point m_timeSinceLastShot;

	//de adaugat vector de bulgari

	void UpdateBulletSpeed();
	void UpdateCooldown(); 
	
};

