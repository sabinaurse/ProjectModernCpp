#include "FriendlyGame.h"
#include <iostream>
#include <stdexcept>

FriendlyGame::FriendlyGame(uint32_t rows, uint32_t cols)
    : Game(rows, cols) {
}

void FriendlyGame::StartGame() {
    m_isGameOver = false;
    InitializeTeams();
    std::cout << "Starting a friendly 2v2 game..." << std::endl;
    StartUpdateLoop();
}

void FriendlyGame::EndGame() {
    StopUpdateLoop();
    m_isGameOver = true;
    std::cout << "Friendly game over!" << std::endl;

    Player* winningTeam = GetWinningTeam();
    if (winningTeam) {
        std::cout << "Team led by " << winningTeam->GetName() << " wins the game!" << std::endl;
    }
    else {
        std::cout << "No team won!" << std::endl;
    }
}

void FriendlyGame::CheckForCollisions() {
    for (const auto& shooterPenguin : m_penguins) {
        for (auto& snowball : shooterPenguin->GetWeapon().GetSnowballs()) {
            if (!snowball.IsActive()) continue;

            for (const auto& targetPenguin : m_penguins) {
                if (targetPenguin == shooterPenguin || !targetPenguin->IsAlive()) continue;

                if (targetPenguin->GetPosition() == snowball.GetPosition()) {
                    targetPenguin->TakeDamage();
                    snowball.Deactivate();

                    std::cout << "Penguin controlled by " << targetPenguin->GetPlayer()->GetName()
                        << " was eliminated!" << std::endl;

                    int team1Alive = 0, team2Alive = 0;
                    for (const auto& penguin : m_penguins) {
                        if (penguin->IsAlive()) {
                            auto player = penguin->GetPlayer();
                            if (player == m_teams[0].first || player == m_teams[0].second) {
                                ++team1Alive;
                            }
                            else if (player == m_teams[1].first || player == m_teams[1].second) {
                                ++team2Alive;
                            }
                        }
                    }

                    if (team1Alive == 0 || team2Alive == 0) {
                        m_isGameOver = true;
                    }
                }
            }
        }
    }

    CheckSnowballToObstacleCollisions();
    CheckSnowballToSnowballCollisions();
}

void FriendlyGame::InitializeTeams() {
    if (m_players.size() != 4) {
        throw std::runtime_error("Friendly games require exactly 4 players.");
    }

    std::cout << "Creating teams for friendly game..." << std::endl;

    m_teams.push_back({ m_players[0].get(), m_players[1].get() });
    m_teams.push_back({ m_players[2].get(), m_players[3].get() });

    InitializePlayers();
}

Player* FriendlyGame::GetWinningTeam() {
    int team1Alive = 0, team2Alive = 0;

    for (const auto& penguin : GetPenguins()) {
        if (penguin->IsAlive()) {
            auto player = penguin->GetPlayer();
            if (player == m_teams[0].first || player == m_teams[0].second) {
                ++team1Alive;
            }
            else if (player == m_teams[1].first || player == m_teams[1].second) {
                ++team2Alive;
            }
        }
    }

    if (team1Alive > 0 && team2Alive == 0) return m_teams[0].first;
    if (team2Alive > 0 && team1Alive == 0) return m_teams[1].first;
    return nullptr;
}
