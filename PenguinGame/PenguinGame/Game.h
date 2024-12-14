#pragma once
#include <vector>
#include <algorithm>
#include <cstdint>
#include "Penguin.h"
#include "Player.h"
#include "GameBoard.h"

using Position = std::pair<std::uint32_t, std::uint32_t>; // maybe uint32_t??
using PlayerList = std::vector<Player*>; 
using PenguinList = std::vector<Penguin*>; 

class Game
{
public:
	void StartGame();
	void EndGame();
	void RestartGame();
	void CheckForCollisions();
	void ShowLeaderboard();
	GameBoard GetBoard() const;

	void AddPenguin(Player* player);
	void AddPlayer(Player* player);
	void InitializePlayers();

	Player* GetWinner();
	Penguin* GetPenguinForPlayer(const Player& player);
	const std::vector<Penguin*>& GetPenguins() const {return m_penguins;}
	std::vector<Player*>& GetPlayers() { return m_players; }
	Player* GetPlayerByName(const std::string& playerName);

	// Gestionarea upgrade-urilor
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

