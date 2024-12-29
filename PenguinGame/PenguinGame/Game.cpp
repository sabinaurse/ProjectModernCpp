#include "Game.h"

const MapGen::GameBoard& Game::GetBoard() const {
    return m_boardManager.GetGameBoard();
}

void Game::StartGame()
{
    m_isGameOver = false;
    InitializePlayers();
    std::cout << "Starting the game..." << std::endl;
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

    for (const auto& penguin : m_penguins) {
        if (!penguin->IsAlive() && penguin->GetEliminationOrder() > lastEliminationOrder) {
            secondPlacePenguin = penguin.get();
            lastEliminationOrder = penguin->GetEliminationOrder();
        }
    }

    if (secondPlacePenguin != nullptr) {
        Player* secondPlacePlayer = secondPlacePenguin->GetPlayer();
        secondPlacePlayer->AddScores(1);
        std::cout << secondPlacePlayer->GetName() << " finishes second!" << std::endl;
    }
}


void Game::RestartGame() {
    m_isGameOver = false;

    for (const auto& penguin : m_penguins) {
        penguin->ResetState();
    }

    m_boardManager.GenerateMap();

    std::cout << "Game restarted!" << std::endl;
}


void Game::ShowLeaderboard()
{
    std::cout << "Leaderboard:" << std::endl;
    std::sort(m_players.begin(), m_players.end(),
        [](const auto& a, const auto& b) { return a->GetPoints() > b->GetPoints(); });

    for (const auto& player : m_players) {
        std::cout << player->GetName() << " - " << player->GetPoints() << " points" << std::endl;
    }
}


void Game::AddPlayer(std::unique_ptr<Player> player) {
    if (std::any_of(m_players.begin(), m_players.end(),
        [&player](const auto& existingPlayer) { return existingPlayer->GetName() == player->GetName(); })) {
        throw std::runtime_error("Player with the same name already exists in the game!");
    }
    m_players.push_back(std::move(player));
}


//void Game::AddPlayer(Player* player)
//{
//    for (const auto& existingPlayer : m_players) {
//        if (existingPlayer->GetName() == player->GetName()) {
//            throw std::runtime_error("Player with the same name already exists in the game!");
//        }
//    }
//    m_players.push_back(player);
//}


void Game::InitializePlayers() {
    const auto& startingPositions = m_boardManager.GetStartingPositions();

    if (m_players.size() > startingPositions.size()) {
        throw std::runtime_error("Too many players for available starting positions!");
    }

    for (size_t i = 0; i < m_players.size(); ++i) {
        auto& player = m_players[i];
        //auto position = startingPositions[i];
        Position position{ startingPositions[i] };

        m_penguins.push_back(std::make_unique<Penguin>(player.get(), position, 500));
        std::cout << "Player " << player->GetName() << " placed at (" << position.first << ", " << position.second << ")." << std::endl;
    }
}

void Game::AddPenguin(std::unique_ptr<Penguin> penguin) {
    m_penguins.push_back(std::move(penguin));
}

Player* Game::GetWinner() {
    Penguin* winnerPenguin = nullptr;

    for (const auto& penguin : m_penguins) {
        if (penguin->IsAlive()) {
            if (winnerPenguin == nullptr) {
                winnerPenguin = penguin.get();
            }
            else {
                return nullptr;
            }
        }
    }

    return winnerPenguin ? winnerPenguin->GetPlayer() : nullptr;
}


Penguin* Game::GetPenguinForPlayer(const Player& player)
{
    for (const auto& penguin : m_penguins) {
        if (penguin->GetPlayer() == &player) {
            return penguin.get();
        }
    }
    return nullptr;
}

void Game::CheckForCollisions()
{
    CheckPenguinCollisions();
    CheckObstacleCollisions();
    CheckSnowballCollisions();
    CheckPenguinToPenguinCollisions();

    for (const auto& penguin : m_penguins) {
        penguin->RemoveInactiveSnowballs();
    }
}

void Game::CheckPenguinCollisions() {
    for (const auto& shooterPenguin : m_penguins) {
        for (auto& snowball : shooterPenguin->GetSnowballs()) {
            if (!snowball.IsActive()) continue;

            for (const auto& targetPenguin : m_penguins) {
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
}

void Game::CheckObstacleCollisions() {
    for (const auto& penguin : m_penguins) {
        for (auto& snowball : penguin->GetSnowballs()) {
            if (!snowball.IsActive()) continue;

            auto pos = snowball.GetPosition();
            if (!m_boardManager.IsWithinBounds(pos.first, pos.second)) {
                snowball.Deactivate();
                continue;
            }

            int cell = m_boardManager.GetCell(pos.first, pos.second);
            switch (cell) {
            case 1:
                m_boardManager.DestroyCell(pos.first, pos.second);
                snowball.Deactivate();
                break;
            case 2:
                snowball.Deactivate();
                break;
            case 3:
                m_boardManager.TriggerExplosion(pos.first, pos.second, 10);
                snowball.Deactivate();
                break;
            default:
                break;
            }
        }
    }
}

void Game::CheckSnowballCollisions() {
    for (size_t i = 0; i < m_penguins.size(); ++i) {
        auto* penguin1 = m_penguins[i].get();
        for (auto& snowball1 : penguin1->GetSnowballs()) {
            if (!snowball1.IsActive()) continue;

            for (size_t j = i + 1; j < m_penguins.size(); ++j) {
                auto* penguin2 = m_penguins[j].get();
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
}

void Game::CheckPenguinToPenguinCollisions() {
    for (size_t i = 0; i < m_penguins.size(); ++i) {
        auto* penguin1 = m_penguins[i].get();

        for (size_t j = i + 1; j < m_penguins.size(); ++j) {
            auto* penguin2 = m_penguins[j].get();

            if (penguin1->GetPosition() == penguin2->GetPosition()) {
                std::cout << "Penguin " << penguin1->GetPlayer()->GetName()
                    << " and Penguin " << penguin2->GetPlayer()->GetName()
                    << " are on the same position (" << penguin1->GetPosition().first
                    << ", " << penguin1->GetPosition().second << ")." << std::endl;
            }
        }
    }
}

void Game::UpgradePlayer(const std::string& playerName, const std::string& upgradeType)
{
    Player* player = GetPlayerByName(playerName);
    if (!player) {
        throw std::runtime_error("Player not found!");
    }

    Penguin* penguin = GetPenguinForPlayer(*player);
    if (!penguin) {
        throw std::runtime_error("Penguin not found for player!");
    }

    if (upgradeType == "bullet_speed") {
        penguin->UpgradeBulletSpeed();
    }
    else if (upgradeType == "cooldown") {
        penguin->UpgradeFireRate();
    }
    else {
        throw std::invalid_argument("Invalid upgrade type!");
    }
}

Player* Game::GetPlayerByName(const std::string& playerName)
{
    for (const auto& player : m_players) {
        if (player->GetName() == playerName) {
            return player.get();
        }
    }
    return nullptr;
}

void Game::AddPlayerToQueue(Player* player) {
    WaitingPlayer waitingPlayer{ player, std::chrono::steady_clock::now() };
    waitingQueue.push(waitingPlayer);

    TryStartMatch(); // Verificam daca putem porni un joc imediat
}

void Game::TryStartMatch() {
    const size_t maxPlayersPerMatch = 4;
    const size_t minPlayersToStart = 1;
    const int waitTimeLimit = 30; // secunde

    std::vector<WaitingPlayer> tempQueue; // Coada temporara pentru jucatorii care raman
    std::vector<Player*> playersForMatch; // Lista jucatorilor pentru un meci

    auto now = std::chrono::steady_clock::now();

    while (!waitingQueue.empty()) {
        const auto& waitingPlayer = waitingQueue.top();
        waitingQueue.pop();

        auto waitTime = std::chrono::duration_cast<std::chrono::seconds>(now - waitingPlayer.joinTime).count();
        playersForMatch.push_back(waitingPlayer.player);

        if (playersForMatch.size() == maxPlayersPerMatch) {
            StartMatch(playersForMatch);
            return;
        }

        if (waitTime >= waitTimeLimit && playersForMatch.size() >= minPlayersToStart) {
            StartMatch(playersForMatch);
            return;
        }

        tempQueue.push_back(waitingPlayer);
    }

    for (const auto& player : tempQueue) {
        waitingQueue.push(player);
    }

    std::cout << "Not enough players to start a match." << std::endl;
}

void Game::StartMatch(const std::vector<Player*>& playersForMatch) {
    std::cout << "Starting a new match with " << playersForMatch.size() << " players." << std::endl;

    Game newGame(m_boardManager.GetGameBoard().GetRows(), m_boardManager.GetGameBoard().GetCols());
    for (auto* player : playersForMatch) {
        newGame.AddPlayer(std::make_unique<Player>(*player));
    }

    activeGames.push_back(std::move(newGame));
    activeGames.back().StartGame();
}



bool Game::IsGameOver() const {
    return m_isGameOver;
}


void Game::UpdateActiveGames() {
    std::cout << "Currently active games: " << activeGames.size() << std::endl;

    for (auto it = activeGames.begin(); it != activeGames.end();) {
        if (it->IsGameOver()) {
            it->EndGame();
            it = activeGames.erase(it);
        }
    }
}
