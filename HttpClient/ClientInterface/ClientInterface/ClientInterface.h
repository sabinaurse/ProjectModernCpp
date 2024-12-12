#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_ClientInterface.h"
#include <QStackedWidget>
#include "LoginPage.h"
#include "MainPage.h"
#include "GamePage.h"

class ClientInterface : public QMainWindow
{
    Q_OBJECT

public:
    ClientInterface(QWidget *parent = nullptr);
    ~ClientInterface();
private slots:
    void onLoginSuccessful(const QString& playerName, int playerScore, int playerPoints);
    void onGameStartRequested();

private:
    Ui::ClientInterfaceClass ui;
    QWidget* centralWidget;
    QStackedWidget* stackedWidget;

    LoginPage* loginPage;
    MainPage* mainPage;
    GamePage* gamePage;

};
