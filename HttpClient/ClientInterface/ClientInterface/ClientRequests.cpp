#include "ClientRequests.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QUrlQuery>

#include<iostream>

ClientRequests::ClientRequests(QObject* parent) : QObject(parent) {
    networkManager = new QNetworkAccessManager(this);
    connect(networkManager, &QNetworkAccessManager::finished, this, &ClientRequests::onReplyFinished);
}

void ClientRequests::CreatePlayer(const QString& name) {
    QUrl url("http://localhost:18080/addPlayer/" + name);
    QNetworkRequest request(url);
    networkManager->get(request);
}

void ClientRequests::GetPlayer(const QString& name) {
    QUrl url("http://localhost:18080/getPlayer/" + name);
    QNetworkRequest request(url);
    networkManager->get(request);
}

void ClientRequests::DeletePlayer(const QString& name) {
    QUrl url("http://localhost:18080/deletePlayer/" + name);
    QNetworkRequest request(url);
    networkManager->deleteResource(request);
}

void ClientRequests::UpdatePlayer(const QString& name, int newScore, int newPoints) {
    QUrl url("http://localhost:18080/updatePlayer");
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject jsonData;
    jsonData["name"] = name;
    jsonData["score"] = newScore;
    jsonData["points"] = newPoints;

    QJsonDocument jsonDoc(jsonData);
    QByteArray jsonDataBytes = jsonDoc.toJson();

    networkManager->post(request, jsonDataBytes);
}

void ClientRequests::GetGameState() {
    QUrl url("http://localhost:18080/getGameState");
    QNetworkRequest request(url);
    networkManager->get(request);
}

void ClientRequests::AddPlayerToGame(const QString& playerName)
{
    QUrl url("http://localhost:18080/addPlayerToGame");

    QJsonObject json;
    json["name"] = playerName;

    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    networkManager->post(request, QJsonDocument(json).toJson());
}

void ClientRequests::GetMap() {
    QUrl url("http://localhost:18080/getMap");
    QNetworkRequest request(url);
    networkManager->get(request);
}

void ClientRequests::StartGame() {
    QUrl url("http://localhost:18080/startGame");
    QNetworkRequest request(url);
    networkManager->post(request, QByteArray());
}

void ClientRequests::ResetGame() {
    QUrl url("http://localhost:18080/resetGame");
    QNetworkRequest request(url);
    networkManager->post(request, QByteArray());
}

void ClientRequests::GetLeaderboard() {
    QUrl url("http://localhost:18080/leaderboard");
    QNetworkRequest request(url);
    networkManager->get(request);
}

void ClientRequests::onReplyFinished(QNetworkReply* reply) {
    qDebug() << "onReplyFinished called.";
    if (reply->error() == QNetworkReply::NoError) {
        QString data = reply->readAll();
        QJsonDocument jsonDoc = QJsonDocument::fromJson(data.toUtf8());

        if (jsonDoc.isObject()) {
            QJsonObject jsonObj = jsonDoc.object();

            if (jsonObj.contains("x") && jsonObj.contains("y")) {
                int x = jsonObj["x"].toInt();
                int y = jsonObj["y"].toInt();

                QString playerName = ClientState::instance().GetCurrentPlayer();

                qDebug() << "Player:" << playerName << "Position:" << x << y;
                qDebug() << "Parsed JSON:" << QJsonDocument(jsonObj).toJson(QJsonDocument::Indented);

                if (!playerName.isEmpty()) {
                    ClientState::instance().UpdatePlayerPosition(playerName, x, y);
                    qDebug() << ClientState::instance().GetPlayerPosition(playerName);
                    emit gameStateUpdated();
                }
                // NU emitem requestCompleted pentru coordonate!
                reply->deleteLater();
                return;
            }
        }

        // Emit `requestCompleted` doar pentru răspunsurile relevante!
        qDebug() << "Emitting requestCompleted with data:" << data;
        emit requestCompleted(data);
    }
    else {
        qDebug() << "Error in reply:" << reply->errorString();
        emit requestFailed(reply->errorString());
    }

    reply->deleteLater();
}


void ClientRequests::MovePlayer(const QString& playerName, const QString& direction) {

    QUrl url("http://localhost:18080/movePlayer");

    QJsonObject json;
    json["playerName"] = playerName;
    json["direction"] = direction;

    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    qDebug() << "Sending move request for player:" << playerName << "with direction:" << direction;

    networkManager->post(request, QJsonDocument(json).toJson());
}

void ClientRequests::Fire(const QString& playerName) {
    QUrl url("http://localhost:18080/fire");

    QJsonObject json;
    json["playerName"] = playerName;

    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    networkManager->post(request, QJsonDocument(json).toJson());
}
