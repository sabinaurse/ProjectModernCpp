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

enum class PowerUpType {
	ExtraLife
};

struct PowerUp {
	Position position;
	PowerUpType type; 
	bool isActive;

	PowerUp(Position pos, PowerUpType powerUpType = PowerUpType::ExtraLife)
		: position(pos), type(powerUpType), isActive(true) {}
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

	static std::string PowerUpTypeToString(PowerUpType type);
	void TrySpawnPowerUp();
	void CheckPenguinPowerUpCollisions();
	const std::vector<PowerUp>& GetPowerUps() const;

	bool IsGameOver() const;
	bool MapUpdated() const;
	void ResetMapUpdateFlag();

	void StartUpdateLoop();
	void StopUpdateLoop();
	void UpdateAllSnowballs();
private:
	void CheckSnowballToPenguinCollisions();
	void CheckSnowballToObstacleCollisions();
	void CheckSnowballToSnowballCollisions();
private:
	BoardManager m_boardManager;
	PlayerList m_players;
	PenguinList m_penguins;
	int m_radius=10;

	std::vector<PowerUp> m_powerUps;
	const size_t m_maxPowerUps{ 3 };
	std::chrono::time_point<std::chrono::steady_clock> m_lastSpawnTime;

	bool m_isGameOver{ false };
	bool m_mapUpdated{ false };
	int m_gameId{ -1 };

	std::thread m_updateThread; 
	std::atomic<bool> m_running{ false }; 

};

