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
	Penguin(Player* player, Position initialPosition, const Weapon& weapon);

	void Move(char direction, const MapGen::GameBoard& gameBoard);
	void Fire();
	void EliminateEnemy();

	Weapon& GetWeapon() { return m_weapon; }
	const Weapon& GetWeapon() const { return m_weapon; }

	void TakeDamage();
	void ResetState();
	bool IsAlive() const { return m_isAlive; }

	Position GetPosition() const { return m_position; }
	Player* GetPlayer() const { return m_player; }  // Metoda pentru a obt jucatorul asociat

	void RemoveInactiveSnowballs(); // -> de sters/mutat in Weapon

	void MarkAsEliminated(int eliminationOrder);
	int GetEliminationOrder() const;

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

	int m_eliminationOrder{ -1 }; // -1 indică faptul că pinguinul nu a fost eliminat încă

};

