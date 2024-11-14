#pragma once
#include <string>
#include <iostream>
#include <vector>
#include "Weapon.h"
#include "Player.h"
#include "Snowball.h"


class Penguin
{
public:
	Penguin(Player* player, std::pair<int, int> initialPosition, int fireRate);

	void Fire(int mouseX, int mouseY, bool isMouseControlled, char keyboardDirection);
	void UpdateWeapon();
	void TakeDamage(int damage);
	bool IsAlive() const { return m_isAlive; }
	void Move(int dx, int dy);
	int GetScore() const { return m_score; }
	std::pair<int, int> GetPosition() const { return m_position; }
	void ResetCharacter();
	void UpgradeFireRate();

	Player* GetPlayer() const { return m_player; }  // Metodă pentru a obține jucătorul asociat
private:
	//Metoda pentru a calcula directia bulgarelui de zapada
	std::pair<float, float> FireDirectionProjectile(int mouseX, int mouseY, bool isMouseControlled, char heyboardDirection);
	void CheckForWeaponUpgrade();

private:
	Player* m_player; // jucătorul care controlează acest pinguin

	int m_lives;
	int m_points;
	int m_score = 0;
	std::pair<int, int> m_position;
	bool m_isAlive;
	Weapon m_weapon;

	int m_weaponBoostCount;
	int m_speedBoostCount;
	std::vector<Snowball> m_snowballs; //Vector de bulgari de zapada
};

