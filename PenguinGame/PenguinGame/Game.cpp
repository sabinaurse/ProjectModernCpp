#include "Game.h"
#include "Gameboard.h"

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

    // Determinam locul 2
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

    InitializePlayers();
}

void Game::AddPlayer(Player* player)
{
    for (const auto& existingPlayer : m_players) {
        if (existingPlayer->GetName() == player->GetName()) {
            throw std::runtime_error("Player with the same name already exists in the game!");
        }
    }
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
                return nullptr; // Nu exista un singur castigator
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


void Game::InitializePlayers() {
    auto startingPositions = m_gameBoard.GetStartingPositions();

    if (m_players.size() > startingPositions.size()) {
        throw std::runtime_error("Too many players for available starting positions!");
    }

    for (size_t i = 0; i < m_players.size(); ++i) {
        auto& player = m_players[i];
        auto position = startingPositions[i]; 

        Penguin* newPenguin = new Penguin(player, position, 4000);
        m_penguins.push_back(newPenguin);

        std::cout << "Player " << player->GetName() << " placed at ("<< position.first << ", " << position.second << ")." << std::endl;
    }
}