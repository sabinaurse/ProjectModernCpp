#include "ClientInterface.h"

ClientInterface::ClientInterface(QWidget *parent)
    : QMainWindow(parent),
    m_clientRequests(new ClientRequests(this)),
    loginPage(new LoginPage(m_clientRequests, this)),
    mainPage(new MainPage(m_clientRequests, this)),
    gamePage(new GamePage(m_clientRequests, this)),
    stackedWidget(new QStackedWidget(this))
{
    resize(800, 600); 

    centralWidget = new QWidget(this);

    stackedWidget->addWidget(loginPage);
    stackedWidget->addWidget(mainPage);
    stackedWidget->addWidget(gamePage);

    loginPage->setMinimumSize(800, 600);
    mainPage->setMinimumSize(800, 600);
    gamePage->setMinimumSize(800, 600);

    setCentralWidget(stackedWidget);

    stackedWidget->setCurrentWidget(loginPage);

    connect(loginPage, &LoginPage::loginSuccessful, this, &ClientInterface::onLoginSuccessful);
    connect(mainPage, &MainPage::startGameRequested, this, &ClientInterface::onGameStartRequested);
}

ClientInterface::~ClientInterface() {
    delete loginPage;
    delete mainPage;
}

void ClientInterface::onLoginSuccessful(const QString& playerName, int playerScore, int playerPoints) 
{
    mainPage->displayPlayerInfo(QString("Name: %1\nScore: %2\nPoints: %3")
        .arg(playerName)
        .arg(playerScore)
        .arg(playerPoints));

    stackedWidget->setCurrentWidget(mainPage);
    qDebug() << "Switched to MainPage.";
}

void ClientInterface::onGameStartRequested() {
    stackedWidget->setCurrentWidget(gamePage);
    qDebug() << "Switched to GamePage";
}
