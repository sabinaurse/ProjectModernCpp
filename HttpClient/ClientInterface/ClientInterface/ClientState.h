#ifndef STATE_H
#define STATE_H

#include <QString>

class ClientState {
public:

    static ClientState& instance();

    void SetCurrentPlayer(const QString& name);
    void SetPlayerScore(int score);
    void SetPlayerPoints(int points);

    QString GetCurrentPlayer() const;
    int GetPlayerScore() const;
    int GetPlayerPoints() const;

private:

    ClientState() : m_playerScore(0), m_playerPoints(0) {}

    QString m_currentPlayer;
    int m_playerScore;
    int m_playerPoints;

    ClientState(const ClientState&) = delete;
    ClientState& operator=(const ClientState&) = delete;
};

#endif 
