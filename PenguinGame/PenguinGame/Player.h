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
	int m_scores;
	int m_lives;
	Position m_position;
};

