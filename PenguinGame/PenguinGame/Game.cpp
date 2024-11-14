#include "Game.h"

void Game::EndGame()
{
    m_isGameOver = true;
    std::cout << "Game Over!" << std::endl;

    Player* winner = GetWinner();
    if (winner != nullptr) 
    {
        winner->AddPoints(200);
        winner->SetScore(2);

        std::cout << winner->GetName() << " wins with " << winner->GetPoints() << " points!" << std::endl;
    }
    else {
        std::cout << "No winner found!" << std::endl;
    }

    // Determinăm locul 2
    Player* secondPlace = nullptr;
    int maxPoints = -1;
    for (auto& player : m_players) {
        if (player != winner) {
            if (player->GetPoints() > maxPoints) {
                secondPlace = player;
                maxPoints = player->GetPoints();
            }
        }
    }

    if (secondPlace != nullptr) {
        secondPlace->SetScore(1);
        std::cout << secondPlace->GetName() << " finishes second with " << secondPlace->GetPoints() << " points!" << std::endl;
    }
}

void Game::StartGame()
{
    m_isGameOver = false;
    std::cout << "Starting the game..." << std::endl;

    // Creăm un pinguin pentru fiecare jucător
    for (auto& player : m_players) {
        AddPenguin(player);
    }
}

void Game::AddPlayer(Player* player)
{
    m_players.push_back(player);
}

Player* Game::GetWinner()
{
    Penguin* winnerPenguin = nullptr;

    for (auto* penguin : m_penguins) {
        if (penguin->IsAlive()) {
            if (winnerPenguin == nullptr)
            {
                winnerPenguin = penguin;
            }
            else 
            {
                return nullptr; // Nu există un singur câștigător
            }
        }
    }

    if (winnerPenguin != nullptr) {
        return winnerPenguin->GetPlayer();
    }

    return nullptr;
}

void Game::AddPenguin(Player* player)
{
    Penguin* newPenguin = new Penguin(player, { 0, 0 }, 4);
    m_penguins.push_back(newPenguin);
}

Penguin* Game::GetPenguinForPlayer(const Player& player)
{
    for (auto* penguin : m_penguins) {
        if (penguin->GetPlayer() == &player) {
            return penguin;
        }
    }
    return nullptr;
}

void Game::ShowLeaderboard()
{
    std::cout << "Leaderboard:" << std::endl;
    std::sort(m_players.begin(), m_players.end(), [](Player* a, Player* b) {
        return a->GetPoints() > b->GetPoints();
        });

    for (auto& player : m_players) {
        std::cout << player->GetName() << " - " << player->GetPoints() << " points" << std::endl;
    }
}
