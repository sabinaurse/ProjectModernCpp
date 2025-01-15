#include "GamePage.h"

GamePage::GamePage(ClientRequests* requests, QWidget* parent)
    : QWidget(parent), m_scene(new QGraphicsScene(this)), m_view(new QGraphicsView(m_scene, this)), m_requests(requests), m_map(new Map())
{
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setAlignment(Qt::AlignCenter);
    layout->addWidget(m_view);

    m_view->setRenderHint(QPainter::Antialiasing);
    m_view->setScene(m_scene);
    m_view->show();

    m_scene->addItem(m_map);
    m_map->setZValue(0);

    m_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    QString playerName = ClientState::instance().GetCurrentPlayer();
    auto position = ClientState::instance().GetPlayerPosition(playerName);

    //Penguin* penguin = new Penguin(m_requests);
    //penguin->setPos(position.first * Map::getCellSize(), position.second * Map::getCellSize());
    //penguin->setZValue(1);
    //m_scene->addItem(penguin);
    //m_penguins[playerName] = penguin;

    debugPrintPenguins();

    connect(m_requests, &ClientRequests::gameStateUpdated, this, &GamePage::onGameStateUpdated);
    connect(m_requests, &ClientRequests::mapReceived, this, &GamePage::onMapReceived);
    connect(m_requests, &ClientRequests::gameEventsReceived, this, &GamePage::onGameEventsReceived);


    //connect(m_requests, &ClientRequests::snowballFired, this, &GamePage::onSnowballFired);

    QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, [this]() {
        m_requests->GetGameState();
        m_requests->GetGameEvents();
        });
    timer->start(200);
}

GamePage::~GamePage() {
    qDeleteAll(m_penguins);
    for (auto& snowballs : m_snowballs) {
        for (Snowball* snowball : snowballs) {
            delete snowball;
        }
    }
    m_snowballs.clear();
}

void GamePage::onMapReceived(const std::vector<std::vector<int>>& mapData, std::unordered_map<int, std::string>& cellTypes) {
    qDebug() << "Received map data:" << mapData.size() << "rows.";
    for (const auto& row : mapData) {
        QString rowData;
        for (int cell : row) {
            rowData += QString::number(cell) + " ";
        }
        qDebug() << rowData;
    }
    if (m_map) {
        m_map->setMapData(mapData, cellTypes);
    }
}

void GamePage::onGameStateUpdated() {
    const auto& playerPositions = ClientState::instance().GetPlayerPositions();
    const auto& snowballPositions = ClientState::instance().GetSnowballPositions();

    qDebug() << "Game state updated. Number of players:" << playerPositions.size();
    qDebug() << "Number of snowballs:" << snowballPositions.size();

    const int cellSize = Map::getCellSize(); 

    for (auto it = playerPositions.cbegin(); it != playerPositions.cend(); ++it) {
        const QString& name = it.key();
        const auto& position = it.value();

        if (!m_penguins.contains(name)) {
            Penguin* newPenguin = new Penguin(m_requests);
            newPenguin->setZValue(1);
            m_scene->addItem(newPenguin);
            m_penguins[name] = newPenguin;
        }

        Penguin* penguin = m_penguins[name];
        penguin->setPos(position.first * cellSize, position.second * cellSize); 
    }

    for (auto it = m_snowballs.begin(); it != m_snowballs.end(); ++it) {
        for (Snowball* snowball : it.value()) {
            m_scene->removeItem(snowball);
            delete snowball;
        }
    }
    m_snowballs.clear();

    for (const auto& snowballData : snowballPositions) {
        const auto& position = snowballData.first;
        const QString& owner = snowballData.second;

        Snowball* newSnowball = new Snowball(position.first * cellSize, position.second * cellSize);
        m_scene->addItem(newSnowball);
        m_snowballs[owner].append(newSnowball);
    }

    qDebug() << "Finished updating players and snowballs. Requesting scene update.";
    m_scene->update();
}



void GamePage::debugPrintPenguins() const {
    qDebug() << "Current Penguins in QMap:";
    for (auto it = m_penguins.cbegin(); it != m_penguins.cend(); ++it) {
        const QString& name = it.key();
        const Penguin* penguin = it.value();
        QPointF position = penguin->scenePos(); // Obține poziția actuală a pinguinului în scenă

        qDebug() << "Player Name:" << name << "| Scene Position:" << position;
    }
    qDebug() << "Total Penguins:" << m_penguins.size();
}


//void GamePage::onSnowballFired(int startX, int startY, const QString& direction, float speed) {
//    int cellSize = Map::getCellSize();
//    int scaledX = startX * cellSize;
//    int scaledY = startY * cellSize;
//
//    qDebug() << "Attempting to add Snowball at scene coordinates:" << scaledX << scaledY;
//
//    Snowball* snowball = new Snowball(scaledX, scaledY);
//
//    if (!m_scene->sceneRect().contains(QPointF(scaledX, scaledY))) {
//        qDebug() << "Snowball is outside the scene rect!";
//    }
//
//    m_scene->addItem(snowball);
//
//    if (m_scene->items().contains(snowball)) {
//        qDebug() << "Snowball successfully added to the scene!";
//    }
//    else {
//        qDebug() << "Failed to add Snowball to the scene.";
//    }
//
//    m_scene->update();
//}

void GamePage::onGameEventsReceived(const QVector<QPair<QPair<QString, QPair<int, int>>, int>> &events)
{
    for (const auto& event : events) {
        const QString& type = event.first.first;  
        const QPair<int, int>& position = event.first.second;
        const int radius= event.second;

        if (type == "destructible_wall") {
            qDebug() << "Wall destroyed at:" << position;
            m_map->updateCell(position.second, position.first, 0 );
        }
        else if (type == "indestructible_wall") {
            qDebug() << "Snowball hit indestructible wall at:" << position;
        }
        else if (type == "bomb") {
            qDebug() << "Bomb exploded at:" << position;
        }
        else {
            qDebug() << "Unknown event type:" << type << "at position:" << position;
        }
    }

    m_scene->update();
}


