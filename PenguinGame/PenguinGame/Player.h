#pragma once
#include <iostream>

class Player
{
private:
	std::string m_name;
	std::string m_password;
	int m_points;
	int m_score;
public:
	Player(const std::string& name, const std::string& password);
	std::string GetName() const;
	int GetPoints() const;
	int GetScore() const;
	void AddPoints(int points);
	void SetScore(int score);
	void UpdateScore();
	void SetName(std::string name);
	~Player() = default;
};

