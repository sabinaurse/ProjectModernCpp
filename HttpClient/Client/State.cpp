#include "State.h"

void ClientState::SetCurrentPlayer(const std::string& name) {
    m_currentPlayer = name;
}

std::string ClientState::GetCurrentPlayer() const {
    return m_currentPlayer;
}
