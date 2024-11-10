#pragma once
#include <string>
#include<iostream>
#include "Weapon.h"

struct Position {
	int x, y;
};

class Penguin
{
public:
	Penguin(Position initialPosition, int fireRate);
	void fire();
	void updateWeapon();
	void takeDamage(int damage);
	bool isAlive() const;
	void move(int dx, int dy);
	int getScore() const;
	Position getPosition() const;
private:
	int m_lives;
	int m_points;
	int m_score;
	Position m_position;
	bool m_isAlive;
	Weapon m_weapon;

};

