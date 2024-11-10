#include "Weapon.h"
#include "Snowball.h"

Weapon::Weapon(int fireRate):
	m_fireRate(fireRate), m_timeSinceLastShot(0)
{

}

void Weapon::update()
{
	if (m_timeSinceLastShot < m_fireRate)
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

void Weapon::setFireRate(int newFireRate)
{
	m_fireRate = newFireRate;
}

void Weapon::resetTimeSinceLastShot()
{
	m_timeSinceLastShot = 0;
}

void Weapon::upgradeReloadSpeed()
{
	if (m_fireRate > 1)
	{
		m_fireRate = m_fireRate / 2;
	}
}