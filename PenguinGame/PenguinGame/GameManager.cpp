#include "GameManager.h"
#include <iostream>

GameManager::GameManager() : m_runningMultigamingLoop(false) {
	std::cout << "Initialized GameManager constructor" << std::endl;
}

void GameManager::AddPlayerToQueue(Player* player) {
	std::lock_guard<std::mutex> lock(m_queueMutex);

	std::cout << "Added player " << player->GetName() << " to the waiting queue." << std::endl;

	std::vector<Player*> playersInQueue;
	bool exists = false;

	while (!m_waitingQueue.empty()) {
		const auto& waitingPlayer = m_waitingQueue.top();
		if (waitingPlayer.player == player) {
			exists = true;
			std::cout << "Player " << player->GetName() << " is already in the waiting queue." << std::endl;
			break;
		}
		playersInQueue.push_back(waitingPlayer.player);
		m_waitingQueue.pop();
	}

	if (!exists) {
		WaitingPlayer waitingPlayer{ player, std::chrono::steady_clock::now() };
		m_waitingQueue.push(waitingPlayer);
		m_playerNameToIdMap[player->GetName()] = -1;
		std::cout << "Player " << player->GetName() << " has been added to the waiting queue." << std::endl;
	}

	for (auto* player : playersInQueue) {
		m_waitingQueue.push({ player, std::chrono::steady_clock::now() });
	}

	RunMultigamingLoop();
}

void GameManager::RunMultigamingLoop() {
	if (m_runningMultigamingLoop.exchange(true)) {
		std::cout << "Multigaming loop is running." << std::endl;
		return; 
	}

	m_multigamingThread = std::thread([this]() {
		std::cout << "Starting multigaming loop." << std::endl;
		while (m_runningMultigamingLoop) {
			TryStartMatch();
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}
		std::cout << "Multigaming loop has ended." << std::endl;
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

		std::cout << "Player has been extracted from the waiting queue:" << waitingPlayer.player->GetName() << std::endl;

		auto waitTime = std::chrono::duration_cast<std::chrono::seconds>(now - waitingPlayer.joinTime).count();
		playersForMatch.push_back(waitingPlayer.player);

		if (playersForMatch.size() == MAX_PLAYERS_PER_MATCH) {
			std::cout << "The match can start with " << playersForMatch.size() << " players." << std::endl;
			StartMatch(playersForMatch);
			return;
		}

		if (waitTime >= WAIT_TIME_LIMIT && playersForMatch.size() >= MIN_PLAYERS_TO_START) {
			std::cout << "The match can start with " << playersForMatch.size() << " players." << std::endl;
			StartMatch(playersForMatch);
			return;
		}

		tempQueue.push_back(waitingPlayer);
	}

	std::cout << "Number of players in match: " << playersForMatch.size() << std::endl;

	for (const auto& player : tempQueue) {
		m_waitingQueue.push(player);
	}
	std::cout << "Size of waiting queue with remaining players: " << m_waitingQueue.size() << std::endl;
	std::cout << "Not enough players to start a match." << std::endl;
}

void GameManager::StartMatch(const std::vector<Player*>& playersForMatch) {
	std::cout << "Starting a match with " << playersForMatch.size() << " players." << std::endl;

	auto newGame = std::make_unique<Game>(MAP_ROWS, MAP_COLUMNS);

	newGame->SetGameId(m_gameCounter);

	std::cout << "Adding players to the match..." << std::endl;
	std::cout << "Number of players in match: " << playersForMatch.size() << std::endl;

	for (auto* player : playersForMatch) {
		player->SetGameId(m_gameCounter);
		newGame->AddPlayer(std::make_unique<Player>(*player));
		m_playerNameToIdMap[player->GetName()] = m_gameCounter;
		std::cout << "Adding player: " << player->GetName() << " to the match with id " << player->GetGameId() << "and id "<< m_playerNameToIdMap[player->GetName()] << std::endl;

	}

	{
		std::lock_guard<std::mutex> lock(m_gamesMutex);
		m_activeGames[m_gameCounter] = std::move(newGame);
		std::cout << "The match has started and has been added to the active matches list." << std::endl;
	}
	m_gameCounter++;
	m_playerAssignedCondition.notify_all();
}

void GameManager::UpdateActiveGames() {
	std::lock_guard<std::mutex> lock(m_gamesMutex);
	std::cout << "Updating active matches." << std::endl;

	for (auto it = m_activeGames.begin(); it != m_activeGames.end();) {
		if (it->second->IsGameOver()) {
			std::cout << "The match with ID " << it->first << " is over. Adding it to the inactive matches list." << std::endl;
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
