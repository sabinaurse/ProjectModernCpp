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

void ClientState::SetCooldownLevel(int cooldown) {
    m_cooldownLevel = cooldown;
}

void ClientState::SetBulletSpeedLevel(int bulletSpeed) {
    m_bulletSpeedLevel = bulletSpeed;
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

int ClientState::GetCooldownLevel() const {
    return m_cooldownLevel;
}

int ClientState::GetBulletSpeedLevel() const {
    return m_bulletSpeedLevel;
}

void ClientState::UpdatePlayerPosition(const QString& name, int x, int y) {
    m_playerPositions[name] = { x, y };
}

std::pair<int,int> ClientState::GetPlayerPosition(const QString& name) const {
    if (m_playerPositions.contains(name)) {
        return m_playerPositions[name];
    }
    return { 0, 0 }; 
}

const QMap<QString, std::pair<int, int>>& ClientState::GetPlayerPositions() const {
    return m_playerPositions;
}

void ClientState::ClearPlayerPositions() {
    m_playerPositions.clear();
}

void ClientState::ClearSnowballPositions() {
    m_snowballPositions.clear();
}

void ClientState::UpdateSnowballPosition(int x, int y, const QString& owner) {
    m_snowballPositions.append({ {x, y}, owner });
}

const QVector<QPair<QPair<int, int>, QString>>& ClientState::GetSnowballPositions() const {
    return m_snowballPositions;
}

void ClientState::ClearPowerUpPositions() {
    m_powerUps.clear();
}

void ClientState::UpdatePowerUpPosition(int x, int y, const QString& type) {
    m_powerUps.append({ QPoint(x, y), type });
}

const QVector<QPair<QPoint, QString>>& ClientState::GetPowerUpPositions() const {
    return m_powerUps;
}
