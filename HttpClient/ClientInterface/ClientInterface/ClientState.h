#pragma once

#include <QString>
#include <QMap>
#include <QPoint>

class ClientState {
public:
    static ClientState& instance();

    void SetCurrentPlayer(const QString& name);
    void SetPlayerScore(int score);
    void SetPlayerPoints(int points);
    void SetCooldownLevel(int cooldown);
    void SetBulletSpeedLevel(int bulletSpeed);

    QString GetCurrentPlayer() const;
    int GetPlayerScore() const;
    int GetPlayerPoints() const;
    int GetCooldownLevel() const;
    int GetBulletSpeedLevel() const;

    void UpdatePlayerPosition(const QString& name, int x, int y);
    std::pair<int, int> GetPlayerPosition(const QString& name) const;
    const QMap<QString, std::pair<int, int>>& GetPlayerPositions() const;
    void ClearPlayerPositions();

    void ClearSnowballPositions();
    void UpdateSnowballPosition(int x, int y, const QString& owner);
    const QVector<QPair<QPair<int, int>, QString>>& GetSnowballPositions() const;

    void ClearPowerUpPositions();
    void UpdatePowerUpPosition(int x, int y, const QString& type);
    const QVector<QPair<QPoint, QString>>& GetPowerUpPositions() const;

private:

    ClientState() : m_playerScore(0), m_playerPoints(0) {}

    QString m_currentPlayer;
    int m_playerScore;
    int m_playerPoints;
    int m_cooldownLevel;
    int m_bulletSpeedLevel;

    QMap<QString, std::pair<int, int>> m_playerPositions;
    QVector<QPair<QPair<int, int>, QString>> m_snowballPositions; 

    QVector<QPair<QPoint, QString>> m_powerUps;

    ClientState(const ClientState&) = delete;
    ClientState& operator=(const ClientState&) = delete;
};
