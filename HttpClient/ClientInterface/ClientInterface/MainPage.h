#ifndef MAINPAGE_H
#define MAINPAGE_H

#include <QWidget>
#include <QLabel>
#include "ClientState.h"
#include "ClientRequests.h"
#include <QVBoxLayout>
#include <QKeyEvent>


class MainPage : public QWidget {
    Q_OBJECT

public:
    explicit MainPage(QWidget* parent = nullptr);
    ~MainPage();

    void displayPlayerInfo(const QString& playerInfo);
    void keyPressEvent(QKeyEvent* event);

private:
    QLabel* playerInfoLabel;  
    ClientRequests* clientRequests;
};

#endif 
