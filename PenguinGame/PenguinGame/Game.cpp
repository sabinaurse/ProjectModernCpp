#include "Game.h"

void Game::EndGame()
{
    m_isGameOver = true;
    std::cout << "Game Over!" << std::endl;
    // Poți salva scorurile sau alte informații aici
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
