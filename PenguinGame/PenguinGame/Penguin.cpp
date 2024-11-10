#include "Penguin.h"

Penguin::Penguin(Position initialPosition,int fireRate):
	m_lives(3),m_points(0),m_score(0),m_position(initialPosition),
		m_isAlive(true),m_weapon(fireRate) {}

void Penguin::fire()
{
	// Verificam daca arma poate trage, adica daca e incarcata
	if (m_weapon.canShoot())
	{
		std::cout << "Penguin fired a snowball from position (" << m_position.x << ", " << m_position.y << ")." << std::endl;
		//Aici s-ar crea un glont (Snowball) in functie de directie
		//Detalii despre directie si crearea glontului ar trebui definite in functie de joc
		m_weapon.resetTimeSinceLastShot();
	}
	else {
		std::cout << "Penguin can't shoot yet. Waiting for reload." << std::endl;
	}
}

void Penguin::updateWeapon()
{
	m_weapon.update();
}

void Penguin::takeDamage(int damage)
{
	m_lives -= damage;
	if (m_lives <= 0) {
		m_isAlive = false;
		std::cout << "Penguin has been defeated and is reset to initial position!" << std::endl;
	    //Resetam pozitiona sau alte atribute, daca e necesar
		m_position = { 0, 0 }; //Schimba pozitia in coltul de pornire, poate fi ajustat la pozitia initiala specifica
		m_lives = 3; //Resetam vietile
		m_points = 0; //Resetam punctele
	}
}

bool Penguin::isAlive() const
{
	return m_isAlive;
}

void Penguin::move(int dx, int dy)
{
	//Actualizam pozitia de baza dx si dy (intelegem dx, dy ca deplasari pe axele x si y)
	m_position.x += dx;
	m_position.y += dy;
	std::cout << "Penguin moved to position (" << m_position.x << ", " << m_position.y << ")." << std::endl;
}

int Penguin::getScore() const
{
	return m_score;
}

Position Penguin::getPosition() const
{
	return m_position;
}

