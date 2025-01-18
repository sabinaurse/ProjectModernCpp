#pragma once
#include <queue>
#include <unordered_map>
#include <memory>
#include <mutex>
#include <chrono>
#include <thread>
#include <atomic>
#include <condition_variable>
#include "Game.h"

constexpr int MAP_ROWS = 26;
constexpr int MAP_COLUMNS = 30;

constexpr size_t MAX_PLAYERS_PER_MATCH = 4;
constexpr size_t MIN_PLAYERS_TO_START = 1;
constexpr int WAIT_TIME_LIMIT = 2;

struct WaitingPlayer {
    Player* player; 
    std::chrono::steady_clock::time_point joinTime; 

    bool operator<(const WaitingPlayer& other) const {
        return player->GetScore() < other.player->GetScore();
    }
};

class GameManager {
public:
    GameManager();
    void AddPlayerToQueue(Player* player);
    void RunMultigamingLoop();
    Game* GetGameById(int playerId);
    int GetPlayerIdByName(const std::string& playerName);
    std::condition_variable& GetPlayerAssignedCondition() {
        return m_playerAssignedCondition;
    }

    std::mutex& GetConditionMutex() {
        return m_conditionMutex;
    }

private:
    void TryStartMatch();
    void UpdateActiveGames();
    void StopMultigamingLoop();
    void StartMatch(const std::vector<Player*>& playersForMatch);
private:
    int m_gameCounter=1;
    std::priority_queue<WaitingPlayer> m_waitingQueue;
    std::unordered_map<int, std::unique_ptr<Game>> m_activeGames;
    std::unordered_map<std::string, int> m_playerNameToIdMap;
    std::mutex m_queueMutex;
    std::mutex m_gamesMutex;

    std::atomic<bool> m_runningMultigamingLoop; // Controlează starea buclei
    std::thread m_multigamingThread;

    std::condition_variable m_playerAssignedCondition;
    std::mutex m_conditionMutex;
};
