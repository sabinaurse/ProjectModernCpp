#ifndef STATE_H
#define STATE_H

#include <QString>
#include <QMap>

class ClientState {
public:

    static ClientState& instance();

    void SetCurrentPlayer(const QString& name);
    void SetPlayerScore(int score);
    void SetPlayerPoints(int points);

    QString GetCurrentPlayer() const;
    int GetPlayerScore() const;
    int GetPlayerPoints() const;

    void UpdatePlayerPosition(const QString& name, int x, int y);
    std::pair<int, int> GetPlayerPosition(const QString& name) const;

private:

    ClientState() : m_playerScore(0), m_playerPoints(0) {}

    QString m_currentPlayer;
    int m_playerScore;
    int m_playerPoints;
    QMap<QString, std::pair<int, int>> m_playerPositions;

    ClientState(const ClientState&) = delete;
    ClientState& operator=(const ClientState&) = delete;
};

#endif 
