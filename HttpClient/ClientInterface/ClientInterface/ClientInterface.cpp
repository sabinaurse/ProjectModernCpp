#include "ClientInterface.h"

ClientInterface::ClientInterface(QWidget *parent)
    : QMainWindow(parent),
    loginPage(new LoginPage(this)),
    mainPage(new MainPage(this))
{
    setCentralWidget(loginPage);

    connect(loginPage, &LoginPage::loginSuccessful, this, &ClientInterface::onLoginSuccessful);
}

ClientInterface::~ClientInterface() {
    delete loginPage;
    delete mainPage;
}

void ClientInterface::onLoginSuccessful(const QString& playerName, int playerScore, int playerPoints) {
    setCentralWidget(mainPage);

    mainPage->displayPlayerInfo(QString("Name: %1\nScore: %2\nPoints: %3")
        .arg(playerName)
        .arg(playerScore)
        .arg(playerPoints));
}
