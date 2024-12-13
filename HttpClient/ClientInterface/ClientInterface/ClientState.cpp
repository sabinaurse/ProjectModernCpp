#include "ClientState.h"

ClientState& ClientState::instance() {
    static ClientState instance;
    return instance;
}

void ClientState::SetCurrentPlayer(const QString& name) {
    m_currentPlayer = name;
}

void ClientState::SetPlayerScore(int score) {
    m_playerScore = score; 
}

void ClientState::SetPlayerPoints(int points) {
    m_playerPoints = points; 
}

QString ClientState::GetCurrentPlayer() const {
    return m_currentPlayer;
}

int ClientState::GetPlayerScore() const {
    return m_playerScore; 
}

int ClientState::GetPlayerPoints() const {
    return m_playerPoints; 
}

void ClientState::UpdatePlayerPosition(const QString& name, int x, int y) {
    m_playerPositions[name] = { x, y };
}

std::pair<int, int> ClientState::GetPlayerPosition(const QString& name) const {
    if (m_playerPositions.contains(name)) {
        return m_playerPositions[name];
    }
    return { 0, 0 }; 
}

