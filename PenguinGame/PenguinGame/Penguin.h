#pragma once
#include <string>
#include <iostream>
#include <vector>
#include "Weapon.h"
#include "Snowball.h"

struct Position {
	int x, y;
};

class Penguin
{
public:
	Penguin(Position initialPosition, int fireRate);
	void fire(int mouseX, int mouseY, bool isMouseControlled, char keyboardDirection);
	void updateWeapon();
	void takeDamage(int damage);
	bool isAlive() const;
	void move(int dx, int dy);
	int getScore() const;
	Position getPosition() const;
private:
	//Metoda pentru a calcula directia bulgarelui de zapada
	std::pair<float, float> fireDirectionProjectile(int mouseX, int mouseY, bool isMouseControlled, char heyboardDirection);
	int m_lives;
	int m_points;
	int m_score;
	Position m_position;
	bool m_isAlive;
	Weapon m_weapon;

	std::vector<Snowball> m_snowballs; //Vector de bulgari de zapada
};

