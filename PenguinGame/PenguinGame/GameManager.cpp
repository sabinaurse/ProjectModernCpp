﻿#include "GameManager.h"
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

	// Rulează bucla principală până când toate jocurile sunt terminate
	while (true) {
		TryStartMatch();
		// UpdateActiveGames();

		 // Oprește bucla dacă nu mai sunt jocuri active și coada e goală
		{
			std::lock_guard<std::mutex> lock(m_queueMutex);
			if (m_waitingQueue.empty() && m_activeGames.empty()) {
				break;
			}
		}

		std::this_thread::sleep_for(delay);
	}

}

void GameManager::TryStartMatch() {
	std::vector<Player*> playersForMatch;
	std::vector<WaitingPlayer> tempQueue;  // Coada temporară pentru jucătorii care rămân
	auto now = std::chrono::steady_clock::now();


	while (!m_waitingQueue.empty()) {
		// Scoatem jucătorul din coadă
		auto waitingPlayer = m_waitingQueue.top();
		m_waitingQueue.pop();

		// Calculăm timpul de așteptare
		auto waitTime = std::chrono::duration_cast<std::chrono::seconds>(now - waitingPlayer.joinTime).count();
		playersForMatch.push_back(waitingPlayer.player);


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

	newGame->SetGameId(m_gameCounter);
	m_gameCounter++;

	for (auto* player : playersForMatch) {
		newGame->AddPlayer(std::make_unique<Player>(*player));
		player->SetGameId(newGame->GetGameId());
		m_playerNameToIdMap[player->GetName()] = player->GetGameId();
		std::cout << "[GameManager] Adăugăm jucătorul " << player->GetName() << " la meci cu id " << player->GetGameId() << std::endl;

	}

	newGame->StartGame();

	{
		std::lock_guard<std::mutex> lock(m_gamesMutex);
		m_activeGames[newGame->GetGameId()] = std::move(newGame);
		std::cout << "[GameManager] Jocul cu ID  a început și a fost adăugat în lista jocurilor active." << std::endl;
	}
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
	std::lock_guard<std::mutex> lock(m_gamesMutex); // Lock pentru siguranța accesului concurent
	auto it = m_activeGames.find(playerId);
	if (it != m_activeGames.end()) {
		return it->second.get();  // Returnează jocul asociat ID-ului
	}
	return nullptr;  // Dacă nu găsește jocul, returnează nullptr
}

int GameManager::GetPlayerIdByName(const std::string& playerName) {
	std::lock_guard<std::mutex> lock(m_queueMutex); // Lock pentru siguranța accesului concurent
	auto it = m_playerNameToIdMap.find(playerName);
	if (it != m_playerNameToIdMap.end()) {
		return it->second;  // Returnează ID-ul găsit
	}
	return -1;  // Dacă nu găsește jucătorul, returnează -1
}
