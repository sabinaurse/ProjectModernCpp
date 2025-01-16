#pragma once

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QUrlQuery>
#include <iostream>

#include "ClientState.h"

class ClientRequests : public QObject {
    Q_OBJECT

private:

    enum class RequestType {
        Unknown,
        CreatePlayer,
        GetPlayer,
        UpdatePlayerPosition,
        GetMap,
        Fire,
        GetGameState, 
    };

    QNetworkAccessManager* networkManager;
    QMap<QNetworkReply*, QString> activeRequests;
    QMap<RequestType, std::function<void(const QString&)>> requestActions;

private:
    RequestType toRequestType(const QString& requestType);
    void updatePlayerPositionFromJson(const QJsonObject& jsonObj);
    void getMapFromJson(const QJsonObject& jsonObj);
    void updateGameStateFromJson(const QJsonObject& jsonObj);
    void initializeRequestActions();

private slots:
    void onReplyFinished(QNetworkReply* reply);

public:
    explicit ClientRequests(QObject* parent = nullptr);

    void CreatePlayer(const QString& name);
    void GetPlayer(const QString& name);
    void DeletePlayer(const QString& name);
    void UpdatePlayer(const QString& name, int newScore, int newPoints);
    void GetGameState(const QString& playerName);
    void AddPlayerToGame(const QString& playerName);
    void GetMap(const QString& playerName);
    void StartGame();
    void ResetGame();
    void GetLeaderboard();
    void MovePlayer(const QString& playerName, const QString& direction);
    void Fire(const QString& playerName);

signals:
    void gameStarted();
    void gameStateUpdated();
    void loginCompleted(const QString& data);
    void mapReceived(const std::vector<std::vector<int>>& mapData, std::unordered_map<int, std::string>& cellTypes);
    void requestCompleted(const QString& response);
    void requestFailed(const QString& error);
    void snowballFired(int startX, int startY, const QString& direction, float speed);
    void mapUpdated(const std::vector<std::vector<int>>& mapData);
};
