#include "MainPage.h"

MainPage::MainPage(QWidget* parent)
    : QWidget(parent), playerInfoLabel(new QLabel(this)) {

    setMinimumSize(400, 300); 

    QGridLayout* layout = new QGridLayout(this);

    layout->setContentsMargins(0, 0, 0, 0); 
    layout->addWidget(playerInfoLabel, 0, 1, Qt::AlignTop | Qt::AlignRight);

    setLayout(layout);
}

MainPage::~MainPage() {
    delete playerInfoLabel;  
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
