#include "ClientInterface.h"

ClientInterface::ClientInterface(QWidget *parent)
    : QMainWindow(parent),
    loginPage(new LoginPage(this)),
    mainPage(new MainPage(this))
{
    resize(800, 600); 

    centralWidget = new QWidget(this);

    mainLayout = new QVBoxLayout(centralWidget);

    mainLayout->addStretch();
    mainLayout->addWidget(loginPage,0,Qt::AlignCenter);
    mainLayout->addWidget(mainPage);
    mainLayout->addStretch();

    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);

    loginPage->show();
    mainPage->hide();

    connect(loginPage, &LoginPage::loginSuccessful, this, &ClientInterface::onLoginSuccessful);
}

ClientInterface::~ClientInterface() {
    delete loginPage;
    delete mainPage;
}

void ClientInterface::onLoginSuccessful(const QString& playerName, int playerScore, int playerPoints) {
    loginPage->hide();

    mainPage->displayPlayerInfo(QString("Name: %1\nScore: %2\nPoints: %3")
        .arg(playerName)
        .arg(playerScore)
        .arg(playerPoints));

    mainPage->show();
}
