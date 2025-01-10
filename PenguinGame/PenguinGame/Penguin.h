#pragma once
#include <string>
#include <iostream>
#include <vector>
#include "Weapon.h"
#include "Player.h"
#include "Snowball.h"
#include "BoardManager.h"

using Position = std::pair<std::int32_t, std::int32_t>;

class Penguin
{
public:
	Penguin(Player* player, Position initialPosition, int fireRate);

	void Move(char direction, const MapGen::GameBoard& gameBoard);

	void Fire();

	void UpgradeBulletSpeed(); // de sters
	void UpgradeFireRate(); // de sters

	void EliminateEnemy();

	void TakeDamage();
	void ResetState();
	bool IsAlive() const { return m_isAlive; }

	Position GetPosition() const { return m_position; }

	float GetBulletSpeed() const; // de sters

	Player* GetPlayer() const { return m_player; }  // Metoda pentru a obt jucatorul asociat

	std::vector<Snowball>& GetSnowballs(); // -> de sters/mutat in Weapon
	const std::vector<Snowball>& GetSnowballs() const; // -> de sters/mutat in Weapon

	void RemoveInactiveSnowballs(); // -> de sters/mutat in Weapon

	void MarkAsEliminated(int eliminationOrder);
	int GetEliminationOrder() const;

private:
	void CheckBulletSpeedUpgrade(); // de sters/mutat

private:
	Player* m_player;
	bool m_isAlive{ true };
	char m_currentDirection{ 'W' };
	Position m_position;
	Position m_initialPosition;

	// Statistici 
	int m_lives{ 3 };
	int m_enemiesEliminated{ 0 };

	// Arme
	Weapon m_weapon;

	float m_bulletSpeed{ 0.25f }; // -> de sters
	bool m_speedBoostApplied{ false }; // -> de sters
	int m_weaponBoostCount{ 0 }; // -> de sters


	int m_eliminationOrder{ -1 }; // -1 indică faptul că pinguinul nu a fost eliminat încă

	std::vector<Snowball> m_snowballs; //-> de mutat in Weapon

};

