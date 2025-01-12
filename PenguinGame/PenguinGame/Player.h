#pragma once
#include <iostream>
#include "GameDatabase.h"

class Player
{
public:
	Player(const std::string& name);
	Player(const game_database::GamePlayer& dbPlayer);
	~Player() = default;
	
	std::string GetName() const;
	int GetPoints() const;
	int GetScore() const;

	void SetScore(int score);
	void SetName(const std::string& name);

	void AddPoints(int points);
	void AddScores(int scores);

	int GetBulletSpeedLevel() const;
	void SetBulletSpeedLevel(int level);

	int GetCooldownLevel() const;
	void SetCooldownLevel(int level);
private:
	std::string m_name;
	int m_points{ 0 };
	int m_score{ 0 };
	int m_bulletSpeedLevel{ 0 };
	int m_cooldownLevel{ 0 };
};

