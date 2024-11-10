#pragma once
#include <iostream>

class Player
{
private:
	std::string m_name;
	std::string m_password;
	int m_points;
	int m_score;
	int m_weaponBoostCount;
	int m_speedBoostCount;
public:
	Player(const std::string& name, const std::string& password);
	std::string GetName() const;
	int GetPoints() const;
	int GetScore() const;

	void addPoints(int points);
	void addScore(int points);

	void checkWeaponBoost();
	void checkSpeedBoost();
	~Player()=default;
};

