#include "ClientRequests.h"

ClientRequests::ClientRequests(QObject* parent) : QObject(parent) {
    networkManager = new QNetworkAccessManager(this);
    connect(networkManager, &QNetworkAccessManager::finished, this, &ClientRequests::onReplyFinished);
    initializeRequestActions();
}

void ClientRequests::initializeRequestActions() {
    requestActions[RequestType::CreatePlayer] = [this](const QString& data) {
        emit loginCompleted(data);  
        };

    requestActions[RequestType::GetPlayer] = [this](const QString& data) {
        emit loginCompleted(data); 
        };

    requestActions[RequestType::UpdatePlayerPosition] = [this](const QString& data) {
        QJsonDocument jsonDoc = QJsonDocument::fromJson(data.toUtf8());
        if (jsonDoc.isObject()) {
            QJsonObject jsonObj = jsonDoc.object();
            updatePlayerPositionFromJson(jsonObj);  
        }
        };
}

void ClientRequests::CreatePlayer(const QString& name) {
    QUrl url("http://localhost:18080/addPlayer/" + name);
    QNetworkRequest request(url);
    QNetworkReply* reply = networkManager->get(request);
    activeRequests.insert(reply, "createPlayer");
}

void ClientRequests::GetPlayer(const QString& name) {
    QUrl url("http://localhost:18080/getPlayer/" + name);
    QNetworkRequest request(url);
    QNetworkReply* reply = networkManager->get(request);
    activeRequests.insert(reply, "getPlayer");
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

    QNetworkReply* reply = networkManager->post(request, jsonDataBytes);
    activeRequests.insert(reply, "updatePlayer");
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

ClientRequests::RequestType ClientRequests::toRequestType(const QString& requestType) {
    if (requestType == "createPlayer") {
        return RequestType::CreatePlayer;
    }
    else if (requestType == "getPlayer") {
        return RequestType::GetPlayer;
    }
    else if (requestType == "updatePlayerPosition") {
        return RequestType::UpdatePlayerPosition;
    }
    else {
        return RequestType::Unknown;
    }
}

void ClientRequests::onReplyFinished(QNetworkReply* reply) {
    QString requestType = activeRequests.value(reply, "Unknown");

    RequestType requestEnum = toRequestType(requestType);

    if (reply->error() == QNetworkReply::NoError) {
        QString data = reply->readAll();

        if (requestActions.contains(requestEnum)) {
            requestActions[requestEnum](data);  
        }
        else {
            qDebug() << "Unknown request type:" << requestType;
            emit requestCompleted(data); 
        }
    }
    else {
        qDebug() << "Error in reply:" << reply->errorString();
        emit requestFailed(reply->errorString());
    }

    activeRequests.remove(reply);
    reply->deleteLater();
}

void ClientRequests::updatePlayerPositionFromJson(const QJsonObject& jsonObj) 
{
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
