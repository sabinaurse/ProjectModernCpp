#ifndef CLIENTREQUESTS_H
#define CLIENTREQUESTS_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonArray>
#include <QJsonObject>
#include <ClientState.h>

class ClientRequests : public QObject {
    Q_OBJECT

public:
    explicit ClientRequests(QObject* parent = nullptr);

    void CreatePlayer(const QString& name);
    void GetPlayer(const QString& name);
    void DeletePlayer(const QString& name);
    void UpdatePlayer(const QString& name, int newScore, int newPoints);
    void GetGameState();
    void AddPlayerToGame(const QString& playerName);
    void GetMap();
    void StartGame();
    void ResetGame();
    void GetLeaderboard();
    void MovePlayer(const QString& playerName, const QString& direction);
    void Fire(const QString& playerName);

signals:
    void gameStateUpdated();
    void loginCompleted(const QString& data);
    void requestCompleted(const QString& response);
    void requestFailed(const QString& error);
private slots:
    void onReplyFinished(QNetworkReply* reply);

private:
    QNetworkAccessManager* networkManager;
    QMap<QNetworkReply*, QString> activeRequests;
};

#endif 
