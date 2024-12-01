#pragma once
#include <string>
#include <iostream>
#include <vector>
#include "Weapon.h"
#include "Player.h"
#include "Snowball.h"

using Position = std::pair<std::uint32_t, std::uint32_t>;

class Penguin
{
public:
	Penguin(Player* player,Position initialPosition, int fireRate);

	void Move(char direction, const GameBoard& board);

	void Fire();
	void UpgradeBulletSpeed(); // Metodă pentru dublarea vitezei gloanțelor
	void UpgradeFireRate();

	bool CollidesWith(Penguin* otherPenguin, GameBoard& gameBoard, const std::vector<Snowball>& snowballs);
	void EliminateEnemy();

	void TakeDamage();
	void ResetState();
	bool IsAlive() const { return m_isAlive; }

	Position GetPosition() const { return m_position; }
	float GetBulletSpeed() const; // Returnează viteza curentă a gloanțelor
	Player* GetPlayer() const { return m_player; }  // Metoda pentru a obt jucatorul asociat
	std::vector<Snowball>& GetSnowballs();
	const std::vector<Snowball>& GetSnowballs() const;
	void RemoveInactiveSnowballs();

	void MarkAsEliminated(int eliminationOrder);
	int GetEliminationOrder() const;

private:
	void CheckBulletSpeedUpgrade();

private:
	Player* m_player;
	bool m_isAlive = true;
	char m_currentDirection = 'W';
	Position m_position;
	Position m_initialPosition;

	// Statistici 
	int m_lives = 3;
	int m_enemiesEliminated = 0;

	// Arme
	Weapon m_weapon;
	float m_bulletSpeed = 0.25f;
	bool m_speedBoostApplied = false;
	int m_weaponBoostCount = 0;
	int m_eliminationOrder = -1; // -1 indică faptul că pinguinul nu a fost eliminat încă
	std::vector<Snowball> m_snowballs;

};

