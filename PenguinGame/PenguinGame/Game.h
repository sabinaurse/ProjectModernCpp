#pragma once
#include <queue>
#include <vector>
#include <chrono>
#include <algorithm>
#include <cstdint>
#include <memory>
#include "Penguin.h"
#include "Player.h"
#include "BoardManager.h"

using Position = std::pair<std::int32_t, std::int32_t>; // maybe uint32_t??
using PlayerList = std::vector<std::unique_ptr<Player>>;
using PenguinList = std::vector<std::unique_ptr<Penguin>>;

struct WaitingPlayer {
	Player* player;
	std::chrono::steady_clock::time_point joinTime;

	// Comparator pentru coada de prioritate
	bool operator<(const WaitingPlayer& other) const {
		return player->GetScore() < other.player->GetScore();
	}
};

class Game
{
public:
	Game() = default;
	Game(uint32_t rows, uint32_t cols)
		: m_boardManager(rows, cols) {}

	void StartGame();
	void EndGame();
	void RestartGame();
	void CheckForCollisions();
	void ShowLeaderboard();
	const MapGen::GameBoard& GetBoard() const;

	void AddPenguin(std::unique_ptr<Penguin> penguin);
	void AddPlayer(std::unique_ptr<Player> player);
	void InitializePlayers();
	Player* GetWinner();
	Penguin* GetPenguinForPlayer(const Player& player);
	const std::vector<std::unique_ptr<Penguin>>& GetPenguins() const { return m_penguins; }
	std::vector<std::unique_ptr<Player>>& GetPlayers() { return m_players; }
	Player* GetPlayerByName(const std::string& playerName);
	void UpgradePlayer(const std::string& playerName, const std::string& upgradeType);

	void AddPlayerToQueue(Player* player);
	void TryStartMatch();
	void UpdateActiveGames();
	bool IsGameOver() const;
private:
	void StartMatch(const std::vector<Player*>& playersForMatch);
	void CheckPenguinCollisions();
	void CheckObstacleCollisions();
	void CheckSnowballCollisions();
	void CheckPenguinToPenguinCollisions();
private:
	BoardManager m_boardManager;
	PlayerList m_players;
	PenguinList m_penguins;
	bool m_isGameOver{ false };

	std::priority_queue<WaitingPlayer> waitingQueue; // Coada de prioritati
	std::vector<Game> activeGames; // jocurile active

};

