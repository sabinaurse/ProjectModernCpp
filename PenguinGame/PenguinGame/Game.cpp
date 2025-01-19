#include "Game.h"

int Game::GetGameId() {
    return m_gameId;
}

void Game::SetGameId(int gameId) {
    m_gameId = gameId;
}

const MapGen::GameBoard& Game::GetBoard() const {
    return m_boardManager.GetGameBoard();
}

void Game::StartGame()
{
    m_isGameOver = false;
    InitializePlayers();
    m_lastSpawnTime = std::chrono::steady_clock::now();
    std::cout << "Starting the game..." << std::endl;
    StartUpdateLoop();
}

void Game::EndGame()
{
    StopUpdateLoop();
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

void Game::AddPlayer(std::unique_ptr<Player> player) {
    if (std::any_of(m_players.begin(), m_players.end(),
        [&player](const auto& existingPlayer) { return existingPlayer->GetName() == player->GetName(); })) {
        std::cout << "Player " << player->GetName() << " is already in the game. We won't add him again." << std::endl;
        return;
    }

    std::cout << "Added player " << player->GetName() << " to the game" << std::endl;
    m_players.push_back(std::move(player));
    std::cout << "Total players in game: " << m_players.size() << std::endl;

}

void Game::InitializePlayers() {
    const auto& startingPositions = m_boardManager.GetStartingPositions();

    if (m_players.size() > startingPositions.size()) {
        throw std::runtime_error("Too many players for available starting positions!");
    }

    for (size_t i = 0; i < m_players.size(); ++i) {

        auto& player = m_players[i];
        Position position{ startingPositions[i] };
        Weapon weapon(player->GetCooldownLevel(), player->GetBulletSpeedLevel());

        m_penguins.push_back(std::make_shared<Penguin>(player.get(), position, weapon));
        std::cout << "Player " << player->GetName() << " placed at (" << position.first << ", " << position.second << ")." << std::endl;
    }
}

void Game::AddPenguin(std::shared_ptr<Penguin> penguin) {
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

Player* Game::GetPlayerByName(const std::string& playerName)
{
    for (const auto& player : m_players) {
        if (player->GetName() == playerName) {
            return player.get();
        }
    }
    return nullptr;
}

void Game::CheckForCollisions()
{
    CheckSnowballToPenguinCollisions();
    CheckSnowballToObstacleCollisions();
    CheckSnowballToSnowballCollisions();
    CheckPenguinPowerUpCollisions();

    for (const auto& penguin : m_penguins) {
        penguin->GetWeapon().RemoveInactiveSnowballs();
    }
}

void Game::CheckSnowballToPenguinCollisions() {
    for (const auto& shooterPenguin : m_penguins) {
        for (auto& snowball : shooterPenguin->GetWeapon().GetSnowballs()) {
            if (!snowball.IsActive()) continue;

            for (const auto& targetPenguin : m_penguins) {
                if (targetPenguin == shooterPenguin || !targetPenguin->IsAlive()) continue;

                if (targetPenguin->GetPosition() == snowball.GetPosition()) {
                    targetPenguin->TakeDamage();
                    snowball.Deactivate();

                    if (!targetPenguin->IsAlive()) {
                        shooterPenguin->GetPlayer()->AddPoints(100);
                        std::cout << "Penguin controlled by " << targetPenguin->GetPlayer()->GetName() << " was defeated!" << std::endl;
                    }
                }
            }
        }
    }
}

void Game::CheckSnowballToObstacleCollisions() {
    for (const auto& penguin : m_penguins) {
        for (auto& snowball : penguin->GetWeapon().GetSnowballs()) {
            if (!snowball.IsActive()) continue;

            auto pos = snowball.GetPosition();
            if (!m_boardManager.IsWithinBounds(pos.first, pos.second)) {
                std::cout << "Snowball out of bounds at (" << pos.first << ", " << pos.second << "). Deactivating." << std::endl;
                snowball.Deactivate();
                continue;
            }

            int cell = m_boardManager.GetCell(pos.first, pos.second);
            switch (cell) {
            case 1: 
                std::cout << "Snowball hit destructible wall at (" << pos.first << ", " << pos.second << "). Destroying wall." << std::endl;
                m_boardManager.DestroyCell(pos.first, pos.second);
                snowball.Deactivate();
                m_mapUpdated = true;
                break;

            case 2:
                std::cout << "Snowball hit indestructible wall at (" << pos.first << ", " << pos.second << "). Deactivating snowball." << std::endl;
                snowball.Deactivate();
                m_mapUpdated = true;
                break;

            case 3: 
                std::cout << "Snowball triggered bomb at (" << pos.first << ", " << pos.second << "). Triggering explosion." << std::endl;
                m_boardManager.TriggerExplosion(pos.first, pos.second, RADIUS, m_penguins);
                snowball.Deactivate();
                m_mapUpdated = true;
                break;

            default:
                std::cout << "Snowball hit unknown cell type (" << cell << ") at (" << pos.first << ", " << pos.second << ")." << std::endl;
                break;
            }
        }
    }
}

void Game::CheckSnowballToSnowballCollisions() {
    for (size_t i = 0; i < m_penguins.size(); ++i) {
        auto* penguin1 = m_penguins[i].get();
        for (auto& snowball1 : penguin1->GetWeapon().GetSnowballs()) {
            if (!snowball1.IsActive()) continue;

            for (size_t j = i + 1; j < m_penguins.size(); ++j) {
                auto* penguin2 = m_penguins[j].get();
                for (auto& snowball2 : penguin2->GetWeapon().GetSnowballs()) {
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

void Game::CheckPenguinPowerUpCollisions() {
    for (auto& penguin : m_penguins) {
        for (auto it = m_powerUps.begin(); it != m_powerUps.end(); ++it) {
            if (penguin->GetPosition() == it->position && it->isActive) {
                if (it->type == PowerUpType::ExtraLife) {
                    std::cout << "ExtraLife Power-Up triggered.\n";
                    penguin->AddLife();
                }

                it->isActive = false;
                m_powerUps.erase(it);
                break;
            }
        }
    }
}

std::string Game::PowerUpTypeToString(PowerUpType type) {
    switch (type) {
    case PowerUpType::ExtraLife:
        return "ExtraLife";
    default:
        return "Unknown";
    }
}

void Game::TrySpawnPowerUp() {
    if (m_powerUps.size() >= MAX_POWER_UPS) {
        return;
    }

    auto now = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - m_lastSpawnTime).count();

    if (elapsed >= 10) {
        uint32_t x = rand() % m_boardManager.GetGameBoard().GetRows();
        uint32_t y = rand() % m_boardManager.GetGameBoard().GetCols();

        if (m_boardManager.GetGameBoard().GetBoard()[x][y] == 0) {
            m_powerUps.emplace_back(Position{ x, y }, PowerUpType::ExtraLife);
            m_lastSpawnTime = now;
            std::cout << "Power-Up (ExtraLife) spawned at (" << x << ", " << y << ").\n";
        }
    }
}


const std::vector<PowerUp>& Game::GetPowerUps() const {
    return m_powerUps;
}

bool Game::IsGameOver() const {
    return m_isGameOver;
}

bool Game::MapUpdated() const {
    return m_mapUpdated;
}

void Game::ResetMapUpdateFlag() {
    m_mapUpdated = false;
}

void Game::StartUpdateLoop() {
    m_running = true;

    m_updateThread = std::thread([this]() {
        while (m_running) {
            auto startTime = std::chrono::steady_clock::now();

            UpdateAllSnowballs(); 

            TrySpawnPowerUp();

            CheckForCollisions();

            auto endTime = std::chrono::steady_clock::now();
            std::chrono::duration<float> elapsed = endTime - startTime;
            if (elapsed < std::chrono::milliseconds(2000)) {
                std::this_thread::sleep_for(std::chrono::milliseconds(16) - elapsed);
            }
        }
        });
}


void Game::StopUpdateLoop() {
    m_running = false;

    if (m_updateThread.joinable()) {
        m_updateThread.join();
    }
}


void Game::UpdateAllSnowballs() {
    for (const auto& penguin : m_penguins) {

        for (auto& snowball : penguin->GetWeapon().GetSnowballs()) {
            if (snowball.IsActive()) {
                snowball.UpdatePosition();
                std::cout << "Snowball updated: (" << snowball.GetPosition().first << ", "
                    << snowball.GetPosition().second << ")" << std::endl;
            }
        }
    }

    for (const auto& penguin : m_penguins) {
        penguin->GetWeapon().RemoveInactiveSnowballs();
    }
}


