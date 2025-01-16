#include "GameManager.h"
#include <iostream>

GameManager::GameManager() {}

void GameManager::AddPlayerToQueue(Player* player) {
    std::lock_guard<std::mutex> lock(m_queueMutex);

    std::cout << "[GameManager] Adăugarea jucătorului " << player->GetName() << " în coadă." << std::endl;

    // Verifică dacă jucătorul există deja în coadă
    std::vector<Player*> playersInQueue;
    bool exists = false;

    // Scoate toți jucătorii temporar pentru verificare
    while (!m_waitingQueue.empty()) {
        const auto& waitingPlayer = m_waitingQueue.top();
        if (waitingPlayer.player == player) {
            exists = true;
            std::cout << "[GameManager] Jucătorul " << player->GetName() << " este deja în coadă. Nu îl mai adăugăm." << std::endl;
            break;
        }
        playersInQueue.push_back(waitingPlayer.player);
        m_waitingQueue.pop();
    }

    // Dacă jucătorul nu există deja în coadă, îl adăugăm
    if (!exists) {
        WaitingPlayer waitingPlayer{ player, std::chrono::steady_clock::now() };
        m_waitingQueue.push(waitingPlayer);
        std::cout << "[GameManager] Jucătorul " << player->GetName() << " a fost adăugat în coadă." << std::endl;
    }

    // Reintroduce jucătorii temporari în coadă
    for (auto* player : playersInQueue) {
        m_waitingQueue.push({ player, std::chrono::steady_clock::now() });
    }
}


void GameManager::RunMultigamingLoop() {
    const auto delay = std::chrono::milliseconds(100); // Pauză între iterații
    std::cout << "[GameManager] Începerea buclei principale de multigaming." << std::endl;

    // Rulează bucla principală până când toate jocurile sunt terminate
    while (true) {
        std::cout << "[GameManager] Încercăm să pornim un meci." << std::endl;
        TryStartMatch();
       // UpdateActiveGames();

        // Oprește bucla dacă nu mai sunt jocuri active și coada e goală
        {
            std::lock_guard<std::mutex> lock(m_queueMutex);
            if (m_waitingQueue.empty() && m_activeGames.empty()) {
                std::cout << "[GameManager] Nu mai sunt jocuri active și coada este goală. Oprirea buclei." << std::endl;
                break;
            }
        }

        std::this_thread::sleep_for(delay);
    }

    std::cout << "[GameManager] Bucla principală de multigaming s-a încheiat." << std::endl;
}

void GameManager::TryStartMatch() {
    std::vector<Player*> playersForMatch;
    std::vector<WaitingPlayer> tempQueue;  // Coada temporară pentru jucătorii care rămân
    auto now = std::chrono::steady_clock::now();

    std::cout << "[GameManager] Verificăm dacă putem porni un nou meci." << std::endl;

    while (!m_waitingQueue.empty()) {
        // Scoatem jucătorul din coadă
        auto waitingPlayer = m_waitingQueue.top();
        m_waitingQueue.pop();

        // Calculăm timpul de așteptare
        auto waitTime = std::chrono::duration_cast<std::chrono::seconds>(now - waitingPlayer.joinTime).count();
        playersForMatch.push_back(waitingPlayer.player);

        std::cout << "[GameManager] Jucătorul " << waitingPlayer.player->GetName()
            << " așteaptă de " << waitTime << " secunde." << std::endl;

        // Dacă avem suficienți jucători pentru meci
        if (playersForMatch.size() == MAX_PLAYERS_PER_MATCH) {
            std::cout << "[GameManager] Meciul poate începe cu " << playersForMatch.size() << " jucători." << std::endl;
            StartMatch(playersForMatch);
            return;
        }

        // Dacă jucătorul așteaptă prea mult și sunt suficienți jucători pentru a începe
        if (waitTime >= WAIT_TIME_LIMIT && playersForMatch.size() >= MIN_PLAYERS_TO_START) {
            std::cout << "[GameManager] Meciul poate începe cu " << playersForMatch.size() << " jucători." << std::endl;
            StartMatch(playersForMatch);
            return;
        }

        // Dacă nu sunt suficienți jucători, adăugăm jucătorul la coada temporară
        tempQueue.push_back(waitingPlayer);
    }

    // Reintroducem jucătorii care nu au fost aleși pentru meci în coada principală
    for (const auto& player : tempQueue) {
        m_waitingQueue.push(player);
    }

    std::cout << "[GameManager] Nu sunt suficienți jucători pentru a începe un meci." << std::endl;
}




void GameManager::StartMatch(const std::vector<Player*>& playersForMatch) {
    std::cout << "[GameManager] Pornim un nou meci cu " << playersForMatch.size() << " jucători." << std::endl;

    auto newGame = std::make_unique<Game>(30, 30);
    for (auto* player : playersForMatch) {
        std::cout << "[GameManager] Adăugăm jucătorul " << player->GetName() << " la meci." << std::endl;
        newGame->AddPlayer(std::make_unique<Player>(*player));
    }

    newGame->StartGame();

    {
        std::lock_guard<std::mutex> lock(m_gamesMutex);
        m_activeGames.push_back(std::move(newGame));
        std::cout << "[GameManager] Jocul a început și a fost adăugat în lista jocurilor active." << std::endl;
    }
}

void GameManager::UpdateActiveGames() {
    std::lock_guard<std::mutex> lock(m_gamesMutex);
    std::cout << "[GameManager] Actualizăm jocurile active." << std::endl;

    for (auto it = m_activeGames.begin(); it != m_activeGames.end();) {
        if ((*it)->IsGameOver()) {
            std::cout << "[GameManager] Jocul s-a încheiat. Îl eliminăm din lista activă." << std::endl;
            (*it)->EndGame();
            it = m_activeGames.erase(it);
        }
        else {
            std::cout << "[GameManager] Jocul este încă activ." << std::endl;
            ++it;
        }
    }
}
