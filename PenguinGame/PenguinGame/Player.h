#pragma once
#include <iostream>

class Player
{
private:
	std::string m_name;
	int m_points;
	int m_score;
public:
	Player(const std::string& name);
	std::string getName();
	int getPoints();
	int getScore();
	~Player();
};

