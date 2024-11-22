#pragma once
#include <iostream>
#include "GameDatabase.h"

class Player
{
private:
	std::string m_name;
	std::string m_password;
	int m_points;
	int m_score;
public:
	Player(const std::string& name, const std::string& password);
	Player(const game_database::GamePlayer& dbPlayer);
	std::string GetName() const;
	int GetPoints() const;
	int GetScore() const;
	void AddPoints(int points);
	void SetScore(int score);
	void UpdateScore();
	void SetName(std::string name);
	~Player() = default;
};

