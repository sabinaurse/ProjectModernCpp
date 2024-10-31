#pragma once

struct Position {
	int x, y;
};

class Penguin
{
private:
	int m_lives;
	int m_points;
	int m_score;
	Position m_position;
	bool m_isAlive;
	// instanta arma
public:
	Penguin(Position initialPosition);
};

