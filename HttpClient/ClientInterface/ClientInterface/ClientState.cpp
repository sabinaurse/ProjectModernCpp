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

