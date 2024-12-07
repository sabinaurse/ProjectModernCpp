#include "MainPage.h"

MainPage::MainPage(QWidget* parent)
    : QWidget(parent), playerInfoLabel(new QLabel(this)) {
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(playerInfoLabel);
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
