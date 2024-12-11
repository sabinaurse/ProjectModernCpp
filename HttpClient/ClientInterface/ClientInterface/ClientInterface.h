#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_ClientInterface.h"
#include "LoginPage.h"
#include "MainPage.h"

class ClientInterface : public QMainWindow
{
    Q_OBJECT

public:
    ClientInterface(QWidget *parent = nullptr);
    ~ClientInterface();
private slots:
    void onLoginSuccessful(const QString& playerName, int playerScore, int playerPoints);

private:
    Ui::ClientInterfaceClass ui;
    LoginPage* loginPage;
    MainPage* mainPage;

};
