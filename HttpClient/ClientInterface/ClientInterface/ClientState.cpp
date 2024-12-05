#include "ClientState.h"

void ClientState::SetCurrentPlayer(const QString& name) {
    m_currentPlayer = name;
}

QString ClientState::GetCurrentPlayer() const {
    return m_currentPlayer;
}

