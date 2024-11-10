#pragma once
#include "Snowball.h"
#include <chrono> // introd in c++ 11, utila pt gestionare timp

class Weapon
{
public:
	Weapon(int fireRate);
	void update();
	bool canShoot();
	void setFireRate(int newFireRate);
	void resetTimeSinceLastShot();
	void upgradeReloadSpeed();
	void doubleSnowballSpeed();
private:
	int m_fireRate;
	int m_timeSinceLastShot;
};

