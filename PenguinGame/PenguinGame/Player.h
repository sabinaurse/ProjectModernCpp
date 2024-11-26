#pragma once
#include <iostream>
#include "GameDatabase.h"

class Player
{
public:
	Player(const std::string& name, const std::string& password);
	Player(const game_database::GamePlayer& dbPlayer);
	
	std::string GetName() const;
	int GetPoints() const;
	int GetScore() const;

	void AddPoints(int points);
	void SetScore(int score);
	void SetName(const std::string &name);
	~Player() = default;
private:
	std::string m_name;
	std::string m_password;
	int m_points=0;
	int m_score=0;
};

