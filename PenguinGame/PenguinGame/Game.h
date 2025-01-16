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

	int GetGameId();   
	void SetGameId(int gameId);

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

	bool IsGameRunning() const;
	bool IsGameOver() const;
	bool MapUpdated() const;
	void ResetMapUpdateFlag();

	void StartUpdateLoop();
	void StopUpdateLoop();
	void UpdateAllSnowballs();
private:
	//void StartMatch(const std::vector<Player*>& playersForMatch); // -> GM

	void CheckSnowballToPenguinCollisions();
	void CheckSnowballToObstacleCollisions();
	void CheckSnowballToSnowballCollisions();
private:
	BoardManager m_boardManager;
	PlayerList m_players;
	PenguinList m_penguins;
	int m_radius=10;

	bool m_gameRunning{ false };
	bool m_isGameOver{ false };
	bool m_mapUpdated{ false };
	int m_gameId{ -1 };

	std::thread m_updateThread; 
	std::atomic<bool> m_running{ false }; 

};

