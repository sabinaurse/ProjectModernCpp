#ifndef MAINPAGE_H
#define MAINPAGE_H

#include <QWidget>
#include <QLabel>
#include "ClientState.h"
#include "ClientRequests.h"
#include "GamePage.h"
#include "ui_MainPage.h"
#include <QMessageBox>
#include <QBoxLayout>
#include <QKeyEvent>


class MainPage : public QWidget {
    Q_OBJECT

public:
    explicit MainPage(ClientRequests* clientRequests, QWidget* parent = nullptr);
    ~MainPage();

    void displayPlayerInfo(const QString& playerInfo);
    void keyPressEvent(QKeyEvent* event);

signals:
    void startGameRequested();

private slots:
    void onStartGameClicked();
    void onRequestCompleted(const QString& response);
    void onRequestFailed(const QString& error);

private:
    Ui::MainPage ui;
    QLabel* m_playerInfoLabel;  
    ClientRequests* m_clientRequests;
};

#endif 
