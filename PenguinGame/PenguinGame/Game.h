#pragma once
#include <queue>
#include <vector>
#include <chrono>
#include <algorithm>
#include <cstdint>
#include <memory>
#include <thread>
#include <atomic>
#include "Penguin.h"
#include "Player.h"
#include "BoardManager.h"

using PlayerList = std::vector<std::unique_ptr<Player>>;
using PenguinList = std::vector<std::shared_ptr<Penguin>>;

struct WaitingPlayer { // -> GM
	Player* player;
	std::chrono::steady_clock::time_point joinTime;

	bool operator<(const WaitingPlayer& other) const {
		return player->GetScore() < other.player->GetScore();
	}
};

struct GameEvent {
	std::string type;
	int x, y;
};


class Game
{
public:
	Game() = default;
	Game(uint32_t rows, uint32_t cols)
		: m_boardManager(rows, cols) {}

	Game(const Game&) = delete;
	Game& operator=(const Game&) = delete;

	Game(Game&&) noexcept = default;
	Game& operator=(Game&&) noexcept = default;

	void StartGame();
	void EndGame();
	void CheckForCollisions();
	const MapGen::GameBoard& GetBoard() const;
	BoardManager& GetBoardManager() { return m_boardManager; }
	const BoardManager& GetBoardManager() const { return m_boardManager; }

	void AddPenguin(std::shared_ptr<Penguin> penguin);
	void AddPlayer(std::unique_ptr<Player> player);
	void InitializePlayers();
	Player* GetWinner();
	Penguin* GetPenguinForPlayer(const Player& player);
	Player* GetPlayerByName(const std::string& playerName);
	const std::vector<std::shared_ptr<Penguin>>& GetPenguins() const { return m_penguins; }
	std::vector<std::unique_ptr<Player>>& GetPlayers() { return m_players; }

	//void AddPlayerToQueue(Player* player); // -> GM
	//void TryStartMatch(); // -> GM
	//void UpdateActiveGames(); // -> GM

	bool IsGameOver() const;

	void StartUpdateLoop();
	void StopUpdateLoop();
	void UpdateAllSnowballs();

	std::vector<GameEvent> GetRecentEvents();
private:
	//void StartMatch(const std::vector<Player*>& playersForMatch); // -> GM

	void CheckSnowballToPenguinCollisions();
	void CheckSnowballToObstacleCollisions();
	void CheckSnowballToSnowballCollisions();
private:
	BoardManager m_boardManager;
	PlayerList m_players;
	PenguinList m_penguins;
	bool m_isGameOver{ false };

	std::vector<GameEvent> m_recentEvents;

	std::priority_queue<WaitingPlayer> waitingQueue; // -> GM
	//std::vector<Game> activeGames; // -> GM

	std::thread m_updateThread; 
	std::atomic<bool> m_running{ false }; 

};

