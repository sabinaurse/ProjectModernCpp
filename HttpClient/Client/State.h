#pragma once
#include <string>

class ClientState {
public:
    void SetCurrentPlayer(const std::string& name);
    std::string GetCurrentPlayer() const;

private:
    std::string m_currentPlayer;
};
