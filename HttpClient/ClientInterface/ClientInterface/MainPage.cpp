#include "MainPage.h"

MainPage::MainPage(ClientRequests* clientRequests,QWidget* parent)
    : QWidget(parent), m_clientRequests(clientRequests), m_playerInfoLabel(new QLabel(this)), m_gameStartTimer(new QTimer(this)) {

    ui.setupUi(this);

    QVBoxLayout* layout = new QVBoxLayout();

    layout->setContentsMargins(0, 0, 0, 0); 
    layout->setSpacing(10); 

    layout->addWidget(m_playerInfoLabel, 0, Qt::AlignTop | Qt::AlignRight);

    layout->addStretch(1);  
    layout->addWidget(ui.startGameButton, 0, Qt::AlignCenter); 
    layout->addWidget(ui.quitGameButton, 0, Qt::AlignCenter);  
    layout->addStretch(1);  

    setLayout(layout);

    connect(ui.startGameButton, &QPushButton::clicked, this, &MainPage::onStartGameClicked);
    connect(m_gameStartTimer, &QTimer::timeout, this, &MainPage::checkGameStart);//
    connect(m_clientRequests, &ClientRequests::requestCompleted, this, &MainPage::onRequestCompleted, Qt::UniqueConnection);
    connect(m_clientRequests, &ClientRequests::requestFailed, this, &MainPage::onRequestFailed, Qt::UniqueConnection);
}

MainPage::~MainPage() {
    delete m_playerInfoLabel;  
}

void MainPage::onStartGameClicked() {
    QString playerName = ClientState::instance().GetCurrentPlayer();
    qDebug() << "Adding player to game:" << playerName;

    m_clientRequests->AddPlayerToGame(playerName);
}

void MainPage::onRequestCompleted(const QString& response) {
   
    if (response.contains("Player added")) {
        qDebug() << "Player successfully added. Sending request to start game.";
        m_gameStartTimer->start(5000);
    }
    if (response.trimmed() == "Game") {
        qDebug() << "Game started successfully.";
        m_gameStartTimer->stop();
        emit startGameRequested();
        m_clientRequests->GetMap(ClientState::instance().GetCurrentPlayer());
    }
}

void MainPage::onRequestFailed(const QString& error) {
    qDebug() << "Request failed:" << error;
    QMessageBox::critical(this, "Request Failed", "An error occurred: " + error);
}

void MainPage::displayPlayerInfo(const QString& playerInfo) {

    QString playerName = ClientState::instance().GetCurrentPlayer(); 
    int playerScore = ClientState::instance().GetPlayerScore(); 
    int playerPoints = ClientState::instance().GetPlayerPoints();
    int cooldownLevel = ClientState::instance().GetCooldownLevel();
    int bulletSpeedLevel = ClientState::instance().GetBulletSpeedLevel();

    m_playerInfoLabel->setText(QString("Name: %1\nScore: %2\nPoints: %3\nCooldown: %4\nBullet Speed: %5")
        .arg(playerName)
        .arg(playerScore)
        .arg(playerPoints)
        .arg(cooldownLevel)
        .arg(bulletSpeedLevel));
}

void MainPage::checkGameStart() {
    QString playerName = ClientState::instance().GetCurrentPlayer();
    if (!playerName.isEmpty()) {
        qDebug() << "Checking if game has started for player:" << playerName;
        m_clientRequests->StartGame();
    }
}
