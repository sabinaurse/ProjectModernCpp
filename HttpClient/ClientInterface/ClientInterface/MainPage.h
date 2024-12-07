#ifndef MAINPAGE_H
#define MAINPAGE_H

#include <QWidget>
#include <QLabel>
#include "ClientState.h"
#include <QVBoxLayout>


class MainPage : public QWidget {
    Q_OBJECT

public:
    explicit MainPage(QWidget* parent = nullptr);
    ~MainPage();

    void displayPlayerInfo(const QString& playerInfo);

private:
    QLabel* playerInfoLabel;  
};

#endif 
