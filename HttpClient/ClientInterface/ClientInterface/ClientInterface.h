#pragma once

#include <QtWidgets/QMainWindow>
#include <QStackedWidget>

#include "ClientRequests.h"
#include "ui_ClientInterface.h"
#include "LoginPage.h"
#include "MainPage.h"
#include "GamePage.h"

class ClientInterface : public QMainWindow
{
    Q_OBJECT

private:
    Ui::ClientInterfaceClass ui;
    QWidget* centralWidget;
    QStackedWidget* stackedWidget;

    ClientRequests* m_clientRequests;
    LoginPage* loginPage;
    MainPage* mainPage;
    GamePage* gamePage;

private slots:
    void onLoginSuccessful(const QString& playerName, int playerScore, int playerPoints, int cooldownLevel, int bulletSpeedLevel);
    void onGameStartRequested();

public:
    ClientInterface(QWidget *parent = nullptr);
    ~ClientInterface();
};
