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

    connect(m_requests, &ClientRequests::snowballFired, this, &GamePage::onSnowballFired);

    QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, [this]() {
        m_requests->GetGameState();
        });
    timer->start(200);
}

GamePage::~GamePage() {
    qDeleteAll(m_penguins);
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

    qDebug() << "Game state updated. Number of players:" << playerPositions.size();

    for (auto it = playerPositions.cbegin(); it != playerPositions.cend(); ++it) {
        const QString& name = it.key();
        const auto& position = it.value();

        qDebug() << "Processing player:" << name << "at position:" << position.first << position.second;

        if (!m_penguins.contains(name)) {
            qDebug() << "Adding new penguin for player:" << name;
            Penguin* newPenguin = new Penguin(m_requests);
            newPenguin->setZValue(1);
            m_scene->addItem(newPenguin);
            m_penguins[name] = newPenguin;
        }

        Penguin* penguin = m_penguins[name];
        qDebug() << "Updating penguin position for player:" << name << "to ("
            << position.first * Map::getCellSize() << ","
            << position.second * Map::getCellSize() << ")";
        penguin->setPos(position.first * Map::getCellSize(), position.second * Map::getCellSize());

        debugPrintPenguins();

        qDebug() << "Penguin scene position (after update):" << penguin->scenePos();
    }

    qDebug() << "Finished updating all players. Requesting scene update.";
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


void GamePage::onSnowballFired(int startX, int startY, const QString& direction, float speed) {
    int cellSize = Map::getCellSize();
    int scaledX = startX * cellSize;
    int scaledY = startY * cellSize;

    qDebug() << "Attempting to add Snowball at scene coordinates:" << scaledX << scaledY;

    Snowball* snowball = new Snowball(scaledX, scaledY, direction, speed);

    if (!m_scene->sceneRect().contains(QPointF(scaledX, scaledY))) {
        qDebug() << "Snowball is outside the scene rect!";
    }

    m_scene->addItem(snowball);

    if (m_scene->items().contains(snowball)) {
        qDebug() << "Snowball successfully added to the scene!";
    }
    else {
        qDebug() << "Failed to add Snowball to the scene.";
    }

    m_scene->update();
}



