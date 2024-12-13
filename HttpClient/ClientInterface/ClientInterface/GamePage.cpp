#include "GamePage.h"

GamePage::GamePage(ClientRequests* requests, QWidget* parent)
    : QWidget(parent), m_scene(new QGraphicsScene(this)), m_view(new QGraphicsView(m_scene, this)),
    m_penguin(nullptr), m_requests(requests)
{
    m_view->setGeometry(0, 0, 800, 600);
    m_view->setRenderHint(QPainter::Antialiasing);
    m_view->setScene(m_scene);
    m_view->show();

    QString playerName = ClientState::instance().GetCurrentPlayer();
    auto position = ClientState::instance().GetPlayerPosition(playerName);

    m_penguin = new Penguin(m_requests);
    m_penguin->setPos(position.first, position.second);
    m_scene->addItem(m_penguin);

    connect(m_requests, &ClientRequests::gameStateUpdated, this, &GamePage::onGameStateUpdated);
}

GamePage::~GamePage() {
    delete m_penguin;
}

void GamePage::onGameStateUpdated() 
{
    QString playerName = ClientState::instance().GetCurrentPlayer();
    auto position = ClientState::instance().GetPlayerPosition(playerName);

    if (m_penguin) {
        m_penguin->setPos(position.first, position.second); 
    }

}
