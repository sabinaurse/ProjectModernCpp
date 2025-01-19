#pragma once

#include <QWidget>
#include <QLabel>
#include <QMessageBox>
#include <QBoxLayout>
#include <QKeyEvent>

#include "ClientState.h"
#include "ClientRequests.h"
#include "GamePage.h"
#include "ui_MainPage.h"

class MainPage : public QWidget {
    Q_OBJECT

public:
    explicit MainPage(ClientRequests* clientRequests, QWidget* parent = nullptr);
    ~MainPage();

    void displayPlayerInfo(const QString& playerInfo);

signals:
    void startGameRequested();

private:
    Ui::MainPage ui;
    QLabel* m_playerInfoLabel;
    ClientRequests* m_clientRequests;
    QTimer* m_gameStartTimer;

private slots:
    void onStartGameClicked();
    void onRequestCompleted(const QString& response);
    void onRequestFailed(const QString& error);
    void checkGameStart();
};


