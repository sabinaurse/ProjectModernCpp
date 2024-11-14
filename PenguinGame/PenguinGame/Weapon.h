#pragma once
#include "Snowball.h"
#include <chrono> // introd in c++ 11, utila pt gestionare timp

class Weapon
{
public:
	Weapon(int fireRate);
	bool CanShoot() const;
	void ResetTimeSinceLastShot();
	void UpgradeFireRate(); // posibil unnecessary daca nu e nevoie de un upgrade special, putem pastra doar snowball speed upgraded
	int GetFireRate() const { return m_fireRate; }
	void SetFireRate(int fireRate);
private:
	int m_fireRate;
	std::chrono::steady_clock::time_point m_timeSinceLastShot;
};

