#include "MainPage.h"

MainPage::MainPage(QWidget* parent)
    : QWidget(parent), clientRequests(new ClientRequests(this)), playerInfoLabel(new QLabel(this)) {

    ui.setupUi(this);

    QVBoxLayout* layout = new QVBoxLayout();

    layout->setContentsMargins(0, 0, 0, 0); 
    layout->setSpacing(10); 

    layout->addWidget(playerInfoLabel, 0, Qt::AlignTop | Qt::AlignRight);

    layout->addStretch(1);  
    layout->addWidget(ui.startGameButton, 0, Qt::AlignCenter); 
    layout->addWidget(ui.quitGameButton, 0, Qt::AlignCenter);  
    layout->addStretch(1);  

    setLayout(layout);

    connect(ui.startGameButton, &QPushButton::clicked, this, &MainPage::onStartGameClicked);

    connect(clientRequests, &ClientRequests::requestCompleted, this, &MainPage::onRequestCompleted);
    connect(clientRequests, &ClientRequests::requestFailed, this, &MainPage::onRequestFailed);
}

MainPage::~MainPage() {
    delete playerInfoLabel;  
}

void MainPage::onStartGameClicked() {
    QString playerName = ClientState::instance().GetCurrentPlayer();
    qDebug() << "Adding player to game:" << playerName;

    clientRequests->AddPlayerToGame(playerName);
}

void MainPage::onRequestCompleted(const QString& response) {
   
    if (response.contains("Player added")) {
        qDebug() << "Player successfully added. Sending request to start game.";

        clientRequests->StartGame();
    }
    else if (response.contains("Game started")) {
        qDebug() << "Game started successfully. Opening GamePage.";
        emit startGameRequested();
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

    playerInfoLabel->setText(QString("Name: %1\nScore: %2\nPoints: %3")
        .arg(playerName)
        .arg(playerScore)
        .arg(playerPoints));  
}

void MainPage::keyPressEvent(QKeyEvent* event) {
    QString playerName = ClientState::instance().GetCurrentPlayer();

    if (event->key() == Qt::Key_F) {
        if (!playerName.isEmpty()) {
            clientRequests->Fire(playerName);
        }
    }
    else {
        QString direction;
        if (event->key() == Qt::Key_W) {
            direction = "W";
        }
        else if (event->key() == Qt::Key_S) {
            direction = "S";
        }
        else if (event->key() == Qt::Key_A) {
            direction = "A";
        }
        else if (event->key() == Qt::Key_D) {
            direction = "D";
        }

        if (!direction.isEmpty() && !playerName.isEmpty()) {
            clientRequests->MovePlayer(playerName, direction);
        }
    }

    QWidget::keyPressEvent(event);
}
