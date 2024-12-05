#include "ClientRequests.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QUrlQuery>

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
    if (reply->error() == QNetworkReply::NoError) {
        emit requestCompleted(reply->readAll());
    }
    else {
        emit requestFailed(reply->errorString());
    }
    reply->deleteLater();
}