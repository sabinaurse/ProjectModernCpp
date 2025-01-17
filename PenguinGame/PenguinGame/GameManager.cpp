#include "GameManager.h"
#include <iostream>

GameManager::GameManager() : m_runningMultigamingLoop(false) {
	std::cout << "[GameManager] Constructor inițializat." << std::endl;
}

void GameManager::AddPlayerToQueue(Player* player) {
	std::lock_guard<std::mutex> lock(m_queueMutex);

	std::cout << "[GameManager] Adăugarea jucătorului " << player->GetName() << " în coadă." << std::endl;

	std::vector<Player*> playersInQueue;
	bool exists = false;

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

	if (!exists) {
		WaitingPlayer waitingPlayer{ player, std::chrono::steady_clock::now() };
		m_waitingQueue.push(waitingPlayer);
		m_playerNameToIdMap[player->GetName()] = -1;
		std::cout << "[GameManager] Jucătorul " << player->GetName() << " a fost adăugat în coadă." << std::endl;
	}

	for (auto* player : playersInQueue) {
		m_waitingQueue.push({ player, std::chrono::steady_clock::now() });
	}

	RunMultigamingLoop();
}

void GameManager::RunMultigamingLoop() {
	if (m_runningMultigamingLoop.exchange(true)) {
		std::cout << "[GameManager] Bucla de multigaming rulează deja." << std::endl;
		return; 
	}

	m_multigamingThread = std::thread([this]() {
		std::cout << "[GameManager] Pornim bucla de multigaming." << std::endl;
		while (m_runningMultigamingLoop) {
			TryStartMatch();
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}
		std::cout << "[GameManager] Bucla de multigaming s-a terminat." << std::endl;
		});
}

void GameManager::StopMultigamingLoop() {
	m_runningMultigamingLoop = false;

	if (m_multigamingThread.joinable()) {
		m_multigamingThread.join();
	}
}


void GameManager::TryStartMatch() {
	std::vector<Player*> playersForMatch;
	std::vector<WaitingPlayer> tempQueue;  
	auto now = std::chrono::steady_clock::now();


	while (!m_waitingQueue.empty()) {
		auto waitingPlayer = m_waitingQueue.top();
		m_waitingQueue.pop();

		std::cout << "[GameManager] Jucător extras din coadă: " << waitingPlayer.player->GetName() << std::endl;

		auto waitTime = std::chrono::duration_cast<std::chrono::seconds>(now - waitingPlayer.joinTime).count();
		playersForMatch.push_back(waitingPlayer.player);

		if (playersForMatch.size() == MAX_PLAYERS_PER_MATCH) {
			std::cout << "[GameManager] Meciul poate începe cu " << playersForMatch.size() << " jucători." << std::endl;
			StartMatch(playersForMatch);
			return;
		}

		if (waitTime >= WAIT_TIME_LIMIT && playersForMatch.size() >= MIN_PLAYERS_TO_START) {
			std::cout << "[GameManager] Meciul poate începe cu " << playersForMatch.size() << " jucători." << std::endl;
			StartMatch(playersForMatch);
			return;
		}

		tempQueue.push_back(waitingPlayer);
	}

	std::cout << "[GameManager] Numărul de jucători în playersForMatch: " << playersForMatch.size() << std::endl;

	for (const auto& player : tempQueue) {
		m_waitingQueue.push(player);
	}
	std::cout << "[GameManager] Dimensiunea cozii după reintroducere: " << m_waitingQueue.size() << std::endl;
	std::cout << "[GameManager] Nu sunt suficienți jucători pentru a începe un meci." << std::endl;
}

void GameManager::StartMatch(const std::vector<Player*>& playersForMatch) {
	std::cout << "[GameManager] Pornim un nou meci cu " << playersForMatch.size() << " jucători." << std::endl;

	auto newGame = std::make_unique<Game>(MAP_ROWS, MAP_COLUMNS);

	newGame->SetGameId(m_gameCounter);

	std::cout << "Adăugăm jucători la meci..." << std::endl;
	std::cout << "[GameManager] Numărul de jucători pentru meci: " << playersForMatch.size() << std::endl;

	for (auto* player : playersForMatch) {
		player->SetGameId(m_gameCounter);
		newGame->AddPlayer(std::make_unique<Player>(*player));
		m_playerNameToIdMap[player->GetName()] = m_gameCounter;
		std::cout << "[GameManager] Adăugăm jucătorul " << player->GetName() << " la meci cu id " << player->GetGameId() << "si id-ul"<< m_playerNameToIdMap[player->GetName()] << std::endl;

	}

	//newGame->StartGame();

	{
		std::lock_guard<std::mutex> lock(m_gamesMutex);
		m_activeGames[m_gameCounter] = std::move(newGame);
		std::cout << "[GameManager] Jocul cu ID  a început și a fost adăugat în lista jocurilor active." << std::endl;
	}
	m_gameCounter++;
	m_playerAssignedCondition.notify_all();
}

void GameManager::UpdateActiveGames() {
	std::lock_guard<std::mutex> lock(m_gamesMutex);
	std::cout << "[GameManager] Actualizăm jocurile active." << std::endl;

	for (auto it = m_activeGames.begin(); it != m_activeGames.end();) {
		if (it->second->IsGameOver()) {
			std::cout << "[GameManager] Jocul cu ID " << it->first << " s-a încheiat. Îl eliminăm din lista activă." << std::endl;
			it->second->EndGame();
			it = m_activeGames.erase(it);
		}
		else {
			++it;
		}
	}

}

Game* GameManager::GetGameById(int playerId) {
	std::lock_guard<std::mutex> lock(m_gamesMutex); 
	auto it = m_activeGames.find(playerId);
	if (it != m_activeGames.end()) {
		return it->second.get();  
	}
	return nullptr;  
}

int GameManager::GetPlayerIdByName(const std::string& playerName) {
	std::lock_guard<std::mutex> lock(m_queueMutex); 
	auto it = m_playerNameToIdMap.find(playerName);
	if (it != m_playerNameToIdMap.end()) {
		return it->second;  
	}
	return -1;  
}
