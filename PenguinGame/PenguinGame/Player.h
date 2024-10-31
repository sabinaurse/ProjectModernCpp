#pragma once
#include <iostream>

struct Position {
	int x, y;
};

class Player
{
private:
	std::string m_name;
	int m_points;
	int m_score;
	int m_lives;
	Position m_position;
public:
	Player(const std::string& name, Position initial_position);
	std::string getName();
	int getPoints();
	int getLives();
	~Player();
};

