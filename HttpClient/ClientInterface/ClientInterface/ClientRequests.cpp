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

    requestActions[RequestType::GetMap] = [this](const QString& data) {
        QJsonDocument jsonDoc = QJsonDocument::fromJson(data.toUtf8());
        if (jsonDoc.isObject()) {
            QJsonObject jsonObj = jsonDoc.object();
            getMapFromJson(jsonObj);
        }
        };

    requestActions[RequestType::Fire] = [this](const QString& data) {
        qDebug() << "Fire response received:" << data;

        QJsonDocument jsonDoc = QJsonDocument::fromJson(data.toUtf8());
        if (jsonDoc.isObject()) {
            QJsonObject jsonObj = jsonDoc.object();

            int startY = jsonObj["startX"].toInt();
            int startX = jsonObj["startY"].toInt();
            QString direction = jsonObj["direction"].toString();
            float speed = jsonObj["bulletSpeed"].toDouble();

            qDebug() << "Emitting snowballFired with:" << startX << startY << direction << speed;

            emit snowballFired(startX, startY, direction, speed);
        }
        else {
            qDebug() << "Invalid JSON response for Fire.";
        }
        };

    requestActions[RequestType::GetGameState] = [this](const QString& data) {
        QJsonDocument jsonDoc = QJsonDocument::fromJson(data.toUtf8());
        if (jsonDoc.isObject()) {
            QJsonObject jsonObj = jsonDoc.object();
            updateGameStateFromJson(jsonObj);
        }
        else {
            qDebug() << "Invalid GameState JSON received.";
        }
        };

    requestActions[RequestType::GetGameEvents] = [this](const QString& data) {
        QJsonDocument jsonDoc = QJsonDocument::fromJson(data.toUtf8());
        if (jsonDoc.isObject()) {
            QJsonObject jsonObj = jsonDoc.object();

            // Extrage evenimentele
            if (jsonObj.contains("events") && jsonObj["events"].isArray()) {
                QJsonArray eventsArray = jsonObj["events"].toArray();
                QVector<QPair<QPair<QString, QPair<int, int>>, int>> events;

                // Dacă nu sunt evenimente, returnăm un vector gol
                if (eventsArray.isEmpty()) {
                    qDebug() << "No game events received.";
                    emit gameEventsReceived(events); // Trimite un vector gol
                    return; // Ieși din funcție
                }

                // Dacă există evenimente, le procesăm
                for (const auto& event : eventsArray) {
                    if (event.isObject()) {
                        QJsonObject eventObj = event.toObject();
                        QString type = eventObj["type"].toString();
                        int y = eventObj["x"].toInt();
                        int x = eventObj["y"].toInt();
                        int details = eventObj["details"].toInt();
                        events.append({ {type,{ x, y}},details });
                    }

                }

                emit gameEventsReceived(events);
            }
            else {
                qDebug() << "No 'events' array found in response.";
                emit gameEventsReceived({}); // Trimite un vector gol în caz că nu există câmpul 'events'
            }
        }
        else {
            qDebug() << "Invalid response format for GetGameEvents.";
            emit gameEventsReceived({}); // Trimite un vector gol în caz de eroare în formatul JSON
        }
    };
}

void ClientRequests::GetGameEvents() {
    QUrl url("http://localhost:18080/getGameEvents");
    QNetworkRequest request(url);
    QNetworkReply* reply = networkManager->get(request);
    activeRequests.insert(reply, "getGameEvents");
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
    QNetworkReply* reply = networkManager->get(request);
    activeRequests.insert(reply, "getGameState");
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
    else if (requestType == "Fire") {
        return RequestType::Fire;
    }
    else if (requestType == "getGameState") {
        return RequestType::GetGameState;
    }
    else if (requestType == "getGameEvents") {
        return RequestType::GetGameEvents;
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

        qDebug() << "Response from server for request type:" << requestType;
        qDebug() << "Response data:" << data;

        if (requestActions.contains(requestEnum)) {
            requestActions[requestEnum](data);
        }
        else {
            qDebug() << "Unknown request type:" << requestType;
            emit requestCompleted(data);
        }
    }
    else {
        QString errorString = reply->errorString();
        qDebug() << "Error in reply:" << errorString;

        if (reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() == 400) {
            qDebug() << "Server error:" << reply->readAll();
        }

        emit requestFailed(errorString);
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

void ClientRequests::updateGameStateFromJson(const QJsonObject& jsonObj) {
    ClientState::instance().ClearPlayerPositions();
    if (jsonObj.contains("players") && jsonObj["players"].isArray()) {
        QJsonArray playersArray = jsonObj["players"].toArray();

        // Șterge pozițiile existente
        ClientState::instance().ClearPlayerPositions();

        // Actualizează pozițiile pentru fiecare jucător
        for (const QJsonValue& playerValue : playersArray) {
            if (playerValue.isObject()) {
                QJsonObject playerObj = playerValue.toObject();
                QString name = playerObj["name"].toString();
                int y = playerObj["x"].toInt();
                int x = playerObj["y"].toInt();

                ClientState::instance().UpdatePlayerPosition(name, x, y);
                qDebug() << "Updated position for player:" << name << "to (" << x << "," << y << ")";
            }
        }

        ClientState::instance().ClearSnowballPositions();

        // Procesare poziții snowballs
        if (jsonObj.contains("snowballs") && jsonObj["snowballs"].isArray()) {
            QJsonArray snowballsArray = jsonObj["snowballs"].toArray();

            for (const QJsonValue& snowballValue : snowballsArray) {
                if (snowballValue.isObject()) {
                    QJsonObject snowballObj = snowballValue.toObject();
                    int y = snowballObj["x"].toInt();
                    int x = snowballObj["y"].toInt();
                    QString owner = snowballObj["owner"].toString();

                    ClientState::instance().UpdateSnowballPosition(x, y, owner);
                    qDebug() << "Updated snowball position:" << owner << "(" << x << "," << y << ")";
                }
            }
        }

        // Emit semnal pentru a notifica schimbarea GameState
        emit gameStateUpdated();
    }
    else {
        qDebug() << "GameState response missing 'players' array.";
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

    QJsonDocument doc(json);
    QByteArray requestData = doc.toJson();

    qDebug() << "Sending request to server:";
    qDebug() << "URL:" << url.toString();
    qDebug() << "Payload:" << requestData;

    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QNetworkReply* reply = networkManager->post(request, QJsonDocument(json).toJson());
    activeRequests.insert(reply, "Fire");
}