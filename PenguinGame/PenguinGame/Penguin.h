#pragma once
#include <string>
#include <iostream>
#include <vector>
#include "Weapon.h"
#include "Player.h"
#include "Snowball.h"

using Position = std::pair<int, int>;

class Penguin
{
public:
	Penguin(Player* player,Position initialPosition, int fireRate);

	void Move(char direction, const GameBoard& board);

	void Fire();
	void UpgradeBulletSpeed(); // Metodă pentru dublarea vitezei gloanțelor
	void UpdateWeapon();
	void UpgradeFireRate();

	bool CollidesWith(Penguin* otherPenguin, GameBoard& gameBoard, const std::vector<Snowball>& snowballs);
	void EliminateEnemy();

	void TakeDamage();
	void ResetCharacter();
	void ResetState();

	bool IsAlive() const { return m_isAlive; }
	int GetScore() const { return m_score; }
	Position GetPosition() const { return m_position; }
	float GetBulletSpeed() const; // Returnează viteza curentă a gloanțelor
	Player* GetPlayer() const { return m_player; }  // Metoda pentru a obt jucatorul asociat
	std::vector<Snowball>& GetSnowballs();
	const std::vector<Snowball>& GetSnowballs() const;
	void RemoveInactiveSnowballs();

private:
	//void CheckForWeaponUpgrade();
	void CheckBulletSpeedUpgrade();

private:
	Player* m_player;
	bool m_isAlive = true;
	char m_currentDirection = 'W';
	Position m_position;
	Position m_initialPosition;

	// Statistici și scor
	int m_lives = 3;
	int m_points = 0;
	int m_score = 0;
	int m_enemiesEliminated = 0;

	// Arme
	Weapon m_weapon;
	float m_bulletSpeed = 0.25f;
	bool m_speedBoostApplied = false;
	int m_weaponBoostCount = 0;
	std::vector<Snowball> m_snowballs;

	// Resetare
	int m_resetCount = 0;
	const int m_maxResets = 3;
};

