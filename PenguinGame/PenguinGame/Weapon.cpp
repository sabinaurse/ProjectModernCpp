#include "Weapon.h"

Weapon::Weapon(int fireRate):
	m_fireRate(fireRate),
	m_timeSinceLastShot(0)
{

}

void Weapon::update()
{
	m_timeSinceLastShot++;
}


bool Weapon::canShoot()
{
	if (m_timeSinceLastShot >= m_fireRate)
	{
		m_timeSinceLastShot = 0;
		return true;
	}
	return false;
}
