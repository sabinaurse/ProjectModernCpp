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
        //throw std::runtime_error("Player with the same name already exists in the game!");
    }
    m_players.push_back(std::move(player));
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

    for (const auto& penguin : m_penguins) {
        penguin->GetWeapon().RemoveInactiveSnowballs();
        //if(!penguin)
            //;
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
                m_boardManager.TriggerExplosion(pos.first, pos.second, m_radius,m_penguins);
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



//void Game::AddPlayerToQueue(Player* player) { // -> GM
//    WaitingPlayer waitingPlayer{ player, std::chrono::steady_clock::now() };
//    waitingQueue.push(waitingPlayer);
//
//    TryStartMatch(); // Verificam daca putem porni un joc imediat
//}
//
//void Game::TryStartMatch() { // -> GM
//    const size_t maxPlayersPerMatch = 4;
//    const size_t minPlayersToStart = 1;
//    const int waitTimeLimit = 30; // secunde
//
//    std::vector<WaitingPlayer> tempQueue; // Coada temporara pentru jucatorii care raman
//    std::vector<Player*> playersForMatch; // Lista jucatorilor pentru un meci
//
//    auto now = std::chrono::steady_clock::now();
//
//    while (!waitingQueue.empty()) {
//        const auto& waitingPlayer = waitingQueue.top();
//        waitingQueue.pop();
//
//        auto waitTime = std::chrono::duration_cast<std::chrono::seconds>(now - waitingPlayer.joinTime).count();
//        playersForMatch.push_back(waitingPlayer.player);
//
//        if (playersForMatch.size() == maxPlayersPerMatch) {
//            StartMatch(playersForMatch);
//            return;
//        }
//
//        if (waitTime >= waitTimeLimit && playersForMatch.size() >= minPlayersToStart) {
//            StartMatch(playersForMatch);
//            return;
//        }
//
//        tempQueue.push_back(waitingPlayer);
//    }
//
//    for (const auto& player : tempQueue) {
//        waitingQueue.push(player);
//    }
//
//    std::cout << "Not enough players to start a match." << std::endl;
//}

//void Game::StartMatch(const std::vector<Player*>& playersForMatch) { // -> GM
//    std::cout << "Starting a new match with " << playersForMatch.size() << " players." << std::endl;
//
//    Game newGame(m_boardManager.GetGameBoard().GetRows(), m_boardManager.GetGameBoard().GetCols());
//    for (auto* player : playersForMatch) {
//        newGame.AddPlayer(std::make_unique<Player>(*player));
//    }
//
//    activeGames.push_back(std::move(newGame));
//    activeGames.back().StartGame();
//}



bool Game::IsGameOver() const {
    return m_isGameOver;
}

bool Game::MapUpdated() const {
    return m_mapUpdated;
}

void Game::ResetMapUpdateFlag() {
    m_mapUpdated = false;
}


//void Game::UpdateActiveGames() { // -> GM
//    std::cout << "Currently active games: " << activeGames.size() << std::endl;
//
//    for (auto it = activeGames.begin(); it != activeGames.end();) {
//        if (it->IsGameOver()) {
//            it->EndGame();
//            it = activeGames.erase(it);
//        }
//    }
//}

void Game::StartUpdateLoop() {
    m_running = true;

    m_updateThread = std::thread([this]() {
        while (m_running) {
            auto startTime = std::chrono::steady_clock::now();

            UpdateAllSnowballs(); // Actualizează pozițiile gloanțurilor

            // Verifică coliziunile și salvează evenimentele
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


