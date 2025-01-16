#pragma once
#include <iostream>
#include "GameDatabase.h"

class Player
{
public:
	Player() = delete;
	Player(const std::string& name);
	Player(const game_database::GamePlayer& dbPlayer);
	~Player() = default;

	//pt multigaming
	//Player(const std::string& name, uint32_t score = 0);

	std::string GetName() const;
	int GetPoints() const;
	int GetScore() const;
	int GetBulletSpeedLevel() const;
	int GetCooldownLevel() const;
	int GetGameId() const;

	void SetName(std::string&& name);
	void SetPoints(uint32_t points);
	void SetScore(uint32_t score);
	void SetBulletSpeedLevel(uint8_t level);
	void SetCooldownLevel(uint8_t level);
	void SetGameId(int8_t gameId);

	void AddPoints(uint32_t points);
	void AddScores(uint32_t scores);

private:
	std::string m_name;
	uint32_t m_points{ 0 };
	uint32_t m_score{ 0 };
	uint8_t m_bulletSpeedLevel{ 0 };
	uint8_t m_cooldownLevel{ 0 };
	int8_t m_gameId{ -1 };
};

