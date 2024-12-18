#pragma once
#include <vector>
#include <algorithm>
#include <cstdint>
#include <memory>
#include "Penguin.h"
#include "Player.h"
#include "GameBoard.h"

using Position = std::pair<std::int32_t, std::int32_t>; // maybe uint32_t??
using PlayerList = std::vector<std::unique_ptr<Player>>;
using PenguinList = std::vector<std::unique_ptr<Penguin>>;

class Game
{
public:
	void StartGame();
	void EndGame();
	void RestartGame();
	void CheckForCollisions();
	void ShowLeaderboard();
	GameBoard GetBoard() const;

	void AddPenguin(std::unique_ptr<Penguin> penguin);
	void AddPlayer(std::unique_ptr<Player> player);
	void InitializePlayers();

	Player* GetWinner();
	Penguin* GetPenguinForPlayer(const Player& player);
	const std::vector<std::unique_ptr<Penguin>>& GetPenguins() const { return m_penguins; }
	std::vector<std::unique_ptr<Player>>& GetPlayers() { return m_players; }
	Player* GetPlayerByName(const std::string& playerName);
	void UpgradePlayer(const std::string& playerName, const std::string& upgradeType);
private:
	void CheckPenguinCollisions();
	void CheckObstacleCollisions();
	void CheckSnowballCollisions();
	void CheckPenguinToPenguinCollisions();
private:
	GameBoard m_gameBoard;
	PlayerList m_players;
	PenguinList m_penguins;
	bool m_isGameOver{ false };
};

