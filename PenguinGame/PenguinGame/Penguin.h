#pragma once
#include <string>
#include <iostream>
#include <vector>
#include "Weapon.h"
#include "Snowball.h"


class Penguin
{
public:
	Penguin(std::pair<int, int> initialPosition, int fireRate);

	void Fire(int mouseX, int mouseY, bool isMouseControlled, char keyboardDirection);
	void UpdateWeapon();
	void TakeDamage(int damage);
	bool IsAlive() const { return m_isAlive; }
	void Move(int dx, int dy);
	int GetScore() const { return m_score; }
	std::pair<int, int> GetPosition() const { return m_position; }
	void ResetCharacter();
	void DoubleSnowballSpeed();

private:
	//Metoda pentru a calcula directia bulgarelui de zapada
	std::pair<float, float> FireDirectionProjectile(int mouseX, int mouseY, bool isMouseControlled, char heyboardDirection);
	void CheckForWeaponUpgrade();

private:
	int m_lives;
	int m_points;
	int m_score=0;
	std::pair<int,int> m_position;
	bool m_isAlive;
	Weapon m_weapon;

	std::vector<Snowball> m_snowballs; //Vector de bulgari de zapada
};

