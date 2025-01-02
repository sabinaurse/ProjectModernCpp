﻿#include "ClientRequests.h"

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

    requestActions[RequestType::GetMap] = [this](const QString& data) {
        QJsonDocument jsonDoc = QJsonDocument::fromJson(data.toUtf8());
        if (jsonDoc.isObject()) {
            QJsonObject jsonObj = jsonDoc.object();
            getMapFromJson(jsonObj);
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
    QNetworkReply* reply = networkManager->get(request);
    activeRequests.insert(reply, "getMap");
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
    else if (requestType == "getMap") {
        return RequestType::GetMap;
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

void ClientRequests::getMapFromJson(const QJsonObject& jsonObj)
{
    if (jsonObj.contains("board") && jsonObj["board"].isArray()) {
        QJsonArray boardArray = jsonObj["board"].toArray();

        qDebug() << "Received board array, number of rows: " << boardArray.size();

        std::vector<std::vector<int>> mapData;

        for (const QJsonValue& rowValue : boardArray) {
            if (!rowValue.isArray()) {
                qDebug() << "Invalid row in 'board.board'. Skipping...";
                continue;
            }

            QJsonArray rowArray = rowValue.toArray();
            std::vector<int> mapRow;

            for (const QJsonValue& cellValue : rowArray) {
                if (!cellValue.isDouble()) {
                    qDebug() << "Invalid cell value in 'board.board'. Skipping...";
                    continue;
                }
                mapRow.push_back(cellValue.toInt());
            }

            mapData.push_back(mapRow);
        }

        qDebug() << "Map received with" << mapData.size() << "rows.";

        if (mapData.empty()) {
            qDebug() << "Map data is empty after parsing!";
            return;
        }

        std::unordered_map<int, std::string> cellTypes;
        if (jsonObj.contains("cellTypes") && jsonObj["cellTypes"].isObject()) {
            QJsonObject cellTypesObj = jsonObj["cellTypes"].toObject();
            for (const QString& typeKey : cellTypesObj.keys()) {
                bool conversionOk;
                int cellType = typeKey.toInt(&conversionOk);
                if (!conversionOk) {
                    qDebug() << "Invalid cell type key in 'cellTypes'. Skipping...";
                    continue;
                }

                cellTypes[cellType] = cellTypesObj[typeKey].toString().toStdString();
            }

            qDebug() << "Cell types metadata parsed.";
            for (const auto& [key, value] : cellTypes) {
                qDebug() << "Type:" << key << ", Description:" << QString::fromStdString(value);
            }
        }

        emit mapReceived(mapData, cellTypes);
    }
    else {
        qDebug() << "No valid 'board' field in JSON.";
    }
}

void ClientRequests::MovePlayer(const QString& playerName, const QString& direction) {
    QUrl url("http://localhost:18080/movePlayer");

    QJsonObject json;
    json["playerName"] = playerName;
    json["direction"] = direction;

    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QNetworkReply* reply = networkManager->post(request, QJsonDocument(json).toJson());
    activeRequests.insert(reply, "updatePlayerPosition");
}

void ClientRequests::Fire(const QString& playerName) {
    QUrl url("http://localhost:18080/fire");

    QJsonObject json;
    json["playerName"] = playerName;

    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    networkManager->post(request, QJsonDocument(json).toJson());
}
