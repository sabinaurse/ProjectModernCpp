#ifndef STATE_H
#define STATE_H

#include <QString>

class ClientState {
public:
    void SetCurrentPlayer(const QString& name);
    QString GetCurrentPlayer() const;

private:
    QString m_currentPlayer;
};

#endif 
