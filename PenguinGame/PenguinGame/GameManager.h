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

constexpr size_t MAX_PLAYERS_PER_MATCH = 4;
constexpr size_t MIN_PLAYERS_TO_START = 2;
constexpr int WAIT_TIME_LIMIT = 30;

struct WaitingPlayer {
    Player* player; // Pointer către jucătorul aflat în așteptare.
    std::chrono::steady_clock::time_point joinTime; // Momentul când jucătorul a fost adăugat în coadă.

    // Operatorul < pentru a defini ordinea în coada de prioritate.
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
private:
    void TryStartMatch();
    void UpdateActiveGames();
    void StartMatch(const std::vector<Player*>& playersForMatch);
private:
    int m_gameCounter=1;
    std::priority_queue<WaitingPlayer> m_waitingQueue;
    std::unordered_map<int, std::unique_ptr<Game>> m_activeGames;
    std::unordered_map<std::string, int> m_playerNameToIdMap;
    std::mutex m_queueMutex;
    std::mutex m_gamesMutex;
};
