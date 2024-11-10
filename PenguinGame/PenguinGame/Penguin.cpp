#include "Penguin.h"

Penguin::Penguin(Position initialPosition,int fireRate):
	m_lives(3),m_points(0),m_score(0),m_position(initialPosition),
		m_isAlive(true),m_weapon(fireRate) {}

void Penguin::fire(int mouseX, int mouseY, bool isMouseControlled, char keyboardDirection)
{
	// Verificam daca arma poate trage
	if (m_weapon.canShoot())
	{
		//Obtinem directia bulgarelui
		auto direction = fireDirectionProjectile(mouseX, mouseY, isMouseControlled, keyboardDirection);

		float dx = direction.first;
		float dy = direction.second;

		if (dx != 0.0f || dy != 0.0f)
		{
			// Definim direcția ca un string, în funcție de direcția calculată
			std::string directionString;

			if (dx > 0) directionString = "right"; // Dreapta
			else if (dx < 0) directionString = "left"; // Stânga
			else if (dy > 0) directionString = "down"; // Jos
			else if (dy < 0) directionString = "up"; // Sus

			//Cream un nou bulgare de zapada
			Snowball newSnowball(std::make_tuple(m_position.x, m_position.y), directionString);
			
			//Adaugam bulgarele in vectorul de bulgari
			m_snowballs.push_back(newSnowball);
		
			std::cout << "Penguin fired a snowball from position (" << m_position.x << ", " << m_position.y << ")." << std::endl;
		}

		//Resetam timerul armei
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

std::pair<float, float> Penguin::fireDirectionProjectile(int mouseX, int mouseY, bool isMouseControlled, char heyboardDirection)
{
	float dx = 0;
	float dy = 0;

	if (isMouseControlled)
	{
		//Calculam directia pe baza pozitiei mouse-ului
		dx = static_cast<float>(mouseX - m_position.x);
		dy = static_cast<float>(mouseY - m_position.y);

		//Normalizam vectorul directiei
		double length = sqrt(dx * dx + dy * dy);
		if (length != 0)
		{
			dx /= static_cast<float>(length);
			dy /= static_cast<float>(length);
		}
	}
	else {
		//Setam directia pe baza tastaturii
		switch (heyboardDirection)
		{
		    case 'W': dy = -1; break; //sus
			case 'S': dy = 1; break;  //jos
			case 'A': dx = -1; break; //stanga
			case 'D': dx = 1; break;  //dreapta
			default: break;
		}
	}

	return std::make_pair(dx, dy);
}

