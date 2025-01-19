#include "GamePage.h"

GamePage::GamePage(ClientRequests* requests, QWidget* parent)
    : QWidget(parent), m_scene(new QGraphicsScene(this)), m_view(new QGraphicsView(m_scene, this)), m_requests(requests), m_map(new Map()), m_gameStateTimer(new QTimer(this))
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

    connect(m_requests, &ClientRequests::gameStateUpdated, this, &GamePage::onGameStateUpdated);
    connect(m_requests, &ClientRequests::mapReceived, this, &GamePage::onMapReceived);
    connect(m_requests, &ClientRequests::mapUpdated, this, [this](const std::vector<std::vector<int>>& mapData) {
        if (!m_map) {
            qDebug() << "Map object is null!";
            return;
        }
        if (mapData.empty()) {
            qDebug() << "Map data is empty!";
            return;
        }
        m_map->setMapData(mapData);
        m_scene->update();
        });

    connect(m_gameStateTimer, &QTimer::timeout, [this]() {
        m_requests->GetGameState(ClientState::instance().GetCurrentPlayer());
        });
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

void GamePage::clearSnowballs() {
    for (auto it = m_snowballs.begin(); it != m_snowballs.end(); ++it) {
        for (Snowball* snowball : it.value()) {
            m_scene->removeItem(snowball);
            delete snowball;
        }
    }
    m_snowballs.clear();
}

void GamePage::updatePenguins(const QMap<QString, std::pair<int, int>>& playerPositions, int cellSize) {
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
}

void GamePage::updateSnowballs(const QVector<QPair<QPair<int, int>, QString>>& snowballPositions, int cellSize) {
    for (const auto& snowballData : snowballPositions) {
        const auto& position = snowballData.first;
        const QString& owner = snowballData.second;

        Snowball* newSnowball = new Snowball(position.first * cellSize, position.second * cellSize);
        m_scene->addItem(newSnowball);

        m_snowballs[owner].append(newSnowball);
    }
}

void GamePage::onGameStateUpdated() {
    const auto& playerPositions = ClientState::instance().GetPlayerPositions();
    const auto& snowballPositions = ClientState::instance().GetSnowballPositions();
    const auto& powerUpPositions = ClientState::instance().GetPowerUpPositions(); 


    qDebug() << "Game state updated. Number of players:" << playerPositions.size();
    qDebug() << "Number of snowballs:" << snowballPositions.size();
    qDebug() << "Number of power-ups:" << powerUpPositions.size();
    
    const int cellSize = Map::getCellSize(); 

    updatePenguins(playerPositions, cellSize); 
    clearSnowballs();          
    updateSnowballs(snowballPositions, cellSize);
    updatePowerUps(powerUpPositions, cellSize);

    qDebug() << "Finished updating players and snowballs. Requesting scene update.";
    m_scene->update();
}

void GamePage::clearPowerUps() {
    for (auto it = m_powerUps.begin(); it != m_powerUps.end(); ++it) {
        m_scene->removeItem(it.value());
        delete it.value();
    }
    m_powerUps.clear();
}

void GamePage::updatePowerUps(const QVector<QPair<QPoint, QString>>& powerUpPositions, int cellSize) {
    clearPowerUps(); 

    for (const auto& powerUpData : powerUpPositions) {
        QPoint position = powerUpData.first;
        QString type = powerUpData.second;

        QGraphicsEllipseItem* powerUpItem = new QGraphicsEllipseItem();

        int powerUpSize = cellSize / 2;

        powerUpItem->setRect(0, 0, powerUpSize, powerUpSize);

        powerUpItem->setPos(position.x() * cellSize + cellSize / 4,
            position.y() * cellSize + cellSize / 4);
        powerUpItem->setZValue(2);

        QBrush brush;
        if (type == "ExtraLife") {
            brush.setColor(Qt::red);
        }
        else {
            brush.setColor(Qt::yellow);
        }

        brush.setStyle(Qt::SolidPattern);
        powerUpItem->setBrush(brush);

        m_scene->addItem(powerUpItem);
        m_powerUps[{position.x(), position.y()}] = powerUpItem;
    }
}

void GamePage::startGameStateTimer() {
    if (!m_gameStateTimer->isActive()) {
        qDebug() << "Starting game state timer.";
        m_gameStateTimer->start(200);
    }
}

void GamePage::stopGameStateTimer() {
    if (m_gameStateTimer->isActive()) {
        qDebug() << "Stopping game state timer.";
        m_gameStateTimer->stop();
    }
}




