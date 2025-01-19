#pragma once
#include "Game.h"
#include <vector>
#include <utility> 

class FriendlyGame : public Game {
public:
    FriendlyGame(uint32_t rows, uint32_t cols);

    void StartGame() override;
    void EndGame() override;
    void CheckForCollisions() override;

private:
    void InitializeTeams();
    Player* GetWinningTeam();

    std::vector<std::pair<Player*, Player*>> m_teams;
};
