#include "Weapon.h"
#include "Snowball.h"

Weapon::Weapon(int fireRate) :
	m_fireRate(fireRate), m_timeSinceLastShot(std::chrono::steady_clock::now())
{

}

bool Weapon::CanShoot() const
{
	auto currentTime = std::chrono::steady_clock::now();
	auto timeSinceLastShot = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - m_timeSinceLastShot).count();
	return timeSinceLastShot >= m_fireRate;
}

void Weapon::ResetTimeSinceLastShot()
{
	m_timeSinceLastShot = std::chrono::steady_clock::now();
}

void Weapon::UpgradeFireRate() // nesigur daca trb si asta sau doar snowball speed?
{
	if (m_fireRate > 100) { // Asiguram un minim pentru m_fireRate, de exemplu 100 ms
		m_fireRate /= 2;
		std::cout << "Weapon fire rate upgraded! New fire rate: " << m_fireRate << " ms" << std::endl;
	}
}

void Weapon::SetFireRate(int fireRate)
{
	m_fireRate = fireRate;
}
