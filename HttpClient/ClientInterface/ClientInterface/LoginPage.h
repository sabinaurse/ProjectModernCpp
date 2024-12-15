#ifndef LOGINPAGE_H
#define LOGINPAGE_H

#include "ClientState.h"
#include "ClientRequests.h"
#include "MainPage.h"
#include "ui_LoginPage.h"

#include <QDebug>
#include <QWidget>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include <QRegularExpression>

class LoginPage : public QWidget {
    Q_OBJECT

public:
    explicit LoginPage(ClientRequests* requests, QWidget* parent = nullptr);
    ~LoginPage();

private slots:
    void onLoginClicked();
    void onRegisterClicked();
    void onRequestCompleted(const QString& data);
    void onRequestFailed(const QString& error);
signals:
    void loginSuccessful(const QString& playerName, int playerScore, int playerPoints);

private:
    Ui::LoginPage ui;  
    ClientRequests* m_clientRequests;
    QRegularExpression m_nameRegex;
};

#endif

