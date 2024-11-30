#include "Game.h"
#include "Gameboard.h"


void Game::StartGame()
{
    m_isGameOver = false;
    std::cout << "Starting the game..." << std::endl;

    InitializePlayers();
}


void Game::EndGame()
{
    m_isGameOver = true;
    std::cout << "Game Over!" << std::endl;

    Player* winner = GetWinner();
    if (winner != nullptr) 
    {
        winner->AddPoints(200);
        winner->AddScores(2);

        std::cout << winner->GetName() << " wins with " << winner->GetPoints() << " points!" << std::endl;
    }
    else 
    {
        std::cout << "No winner found!" << std::endl;
    }

    // Determină locul 2 pe baza ultimei eliminări
    Penguin* secondPlacePenguin = nullptr;
    int lastEliminationOrder = -1;

    for (auto* penguin : m_penguins) {
        if (!penguin->IsAlive() && penguin->GetEliminationOrder() > lastEliminationOrder) {
            secondPlacePenguin = penguin;
            lastEliminationOrder = penguin->GetEliminationOrder();
        }
    }

    if (secondPlacePenguin != nullptr) {
        Player* secondPlacePlayer = secondPlacePenguin->GetPlayer();
        secondPlacePlayer->AddScores(1); // Locul 2 primește 1 punct
        std::cout << secondPlacePlayer->GetName() << " finishes second!" << std::endl;
    }
}


void Game::RestartGame() {
    m_isGameOver = false;

    for (auto* penguin : m_penguins) {
        penguin->ResetState();
    }

    m_gameBoard.InitializeBoard();

    std::cout << "Game restarted!" << std::endl;
}


void Game::ShowLeaderboard()
{
    std::cout << "Leaderboard:" << std::endl;
    auto comparePlayers = [](const auto& a, const auto& b) {
        return a->GetPoints() > b->GetPoints();
        };
    std::sort(m_players.begin(), m_players.end(), comparePlayers);

    for (auto& player : m_players) {
        std::cout << player->GetName() << " - " << player->GetPoints() << " points" << std::endl;
    }
}

GameBoard Game::GetBoard() const{
    return m_gameBoard; 
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


void Game::InitializePlayers() {
    auto startingPositions = m_gameBoard.GetStartingPositions();

    if (m_players.size() > startingPositions.size()) {
        throw std::runtime_error("Too many players for available starting positions!");
    }

    for (size_t i = 0; i < m_players.size(); ++i) {
        auto& player = m_players[i];
        //auto position = startingPositions[i];
        Position position{ startingPositions[i] };

        Penguin* newPenguin = new Penguin{ player, position, 500 };
        m_penguins.push_back(newPenguin);

        std::cout << "Player " << player->GetName() << " placed at (" << position.first << ", " << position.second << ")." << std::endl;
    }
}


void Game::AddPenguin(Player* player)
{
    Penguin* newPenguin = new Penguin(player, { 0, 0 }, 3);
    m_penguins.push_back(newPenguin);
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


Penguin* Game::GetPenguinForPlayer(const Player& player)
{
    for (auto* penguin : m_penguins) {
        if (penguin->GetPlayer() == &player) {
            return penguin;
        }
    }
    return nullptr;
}


void Game::CheckForCollisions() {
    // 1. Coliziuni intre gloante si pinguini
    for (auto* shooterPenguin : m_penguins) {
        for (auto& snowball : shooterPenguin->GetSnowballs()) {
            if (!snowball.IsActive()) continue;

            // Verificam daca loveste alt pinguin
            for (auto* targetPenguin : m_penguins) {
                if (targetPenguin == shooterPenguin || !targetPenguin->IsAlive()) continue;

                if (targetPenguin->GetPosition() == snowball.GetPosition()) {
                    targetPenguin->TakeDamage();
                    snowball.Deactivate();

                    if (!targetPenguin->IsAlive()) {
                        std::cout << "Penguin controlled by " << targetPenguin->GetPlayer()->GetName() << " was defeated!" << std::endl;
                    }
                }
            }
        }
    }

    // 2. Coliziuni intre gloante ai obstacole
    for (const auto& penguin : m_penguins) {
        for (auto& snowball : penguin->GetSnowballs()) {
            if (!snowball.IsActive()) continue;

            Position pos{ snowball.GetPosition() };
            if (!m_gameBoard.IsWithinBounds(pos.first, pos.second)) {
                snowball.Deactivate();
                continue;
            }

            Cell cell = m_gameBoard.GetCell(pos.first, pos.second);
            switch (cell) {
            case Cell::Destructible_Wall:
                m_gameBoard.DestroyCell(pos.first, pos.second);
                snowball.Deactivate();
                break;
            case Cell::Indestructible_Wall:
                snowball.Deactivate();
                break;
            case Cell::Hidden_Bomb:
                m_gameBoard.TriggerExplosion(pos.first, pos.second, m_penguins);
                snowball.Deactivate();
                break;
            default:
                break;
            }
        }
    }

    // 3. Coliziuni intre gloante (de la pinguini diferiti)
    for (size_t i = 0; i < m_penguins.size(); ++i) {
        auto* penguin1 = m_penguins[i];
        for (auto& snowball1 : penguin1->GetSnowballs()) {
            if (!snowball1.IsActive()) continue;

            for (size_t j = i + 1; j < m_penguins.size(); ++j) {
                auto* penguin2 = m_penguins[j];
                for (auto& snowball2 : penguin2->GetSnowballs()) {
                    if (!snowball2.IsActive()) continue;

                    if (snowball1.GetPosition() == snowball2.GetPosition()) {
                        snowball1.Deactivate();
                        snowball2.Deactivate();
                        std::cout << "Two snowballs collided!" << std::endl;
                    }
                }
            }
        }
    }

    // 4. Eliminam gloantele inactive din fiecare pinguin
    for (auto* penguin : m_penguins) {
        penguin->RemoveInactiveSnowballs();
    }
}



