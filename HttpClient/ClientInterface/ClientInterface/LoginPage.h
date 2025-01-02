#pragma once

#include <QDebug>
#include <QWidget>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include <QRegularExpression>
#include <QMessageBox>
#include <iostream>

#include "ClientState.h"
#include "ClientRequests.h"
#include "MainPage.h"
#include "ui_LoginPage.h"

class LoginPage : public QWidget {
    Q_OBJECT

private:
    Ui::LoginPage ui;
    ClientRequests* m_clientRequests;
    QRegularExpression m_nameRegex;

private slots:
    void onLoginClicked();
    void onRegisterClicked();
    void onRequestCompleted(const QString& data);
    void onRequestFailed(const QString& error);

public:
    explicit LoginPage(ClientRequests* requests, QWidget* parent = nullptr);
    ~LoginPage();

signals:
signals:
    void loginSuccessful(const QString& playerName, int playerScore, int playerPoints, int cooldownLevel, int bulletSpeedLevel);
};


