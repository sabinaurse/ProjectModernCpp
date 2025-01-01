#include "GamePage.h"

GamePage::GamePage(ClientRequests* requests, QWidget* parent)
    : QWidget(parent), m_scene(new QGraphicsScene(this)), m_view(new QGraphicsView(m_scene, this)),
    m_penguin(nullptr), m_requests(requests), m_map(new Map())
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

    m_penguin = new Penguin(m_requests);
    m_penguin->setPos(position.first * Map::getCellSize(), position.second * Map::getCellSize());
    m_penguin->setZValue(1);
    m_scene->addItem(m_penguin);

    connect(m_requests, &ClientRequests::gameStateUpdated, this, &GamePage::onGameStateUpdated);
    connect(m_requests, &ClientRequests::mapReceived, this, &GamePage::onMapReceived);
}

GamePage::~GamePage() {
    delete m_penguin;
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

void GamePage::onGameStateUpdated() 
{
    QString playerName = ClientState::instance().GetCurrentPlayer();
    auto position = ClientState::instance().GetPlayerPosition(playerName);

    qDebug() << "Player:" << playerName << "Position:" << position.first << position.second;

    if (m_penguin) {
        m_penguin->setPos(position.first * Map::getCellSize(), position.second * Map::getCellSize());
    }
}


