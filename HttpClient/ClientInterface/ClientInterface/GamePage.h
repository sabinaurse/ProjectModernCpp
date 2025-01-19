#pragma once

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QObject>
#include <QVBoxLayout>
#include <unordered_map>
#include <QString>
#include <QMap>
#include <QTimer>
#include <memory>

#include "ClientRequests.h"
#include "ClientState.h"
#include "Penguin.h"
#include "Map.h"
#include "Snowball.h"

class GamePage : public QWidget {
    Q_OBJECT

public:
    explicit GamePage(ClientRequests* requests, QWidget* parent = nullptr);
    void startGameStateTimer();
    void clearSnowballs();
    void updatePenguins(const QMap<QString, std::pair<int, int>>& playerPositions, int cellSize);
    void updateSnowballs(const QVector<QPair<QPair<int, int>, QString>>& snowballPositions, int cellSize);
    void stopGameStateTimer();

    void updatePowerUps(const QVector<QPair<QPoint, QString>>& powerUpPositions, int cellSize); 
    void clearPowerUps(); 

    ~GamePage();

private:
    QGraphicsScene* m_scene;
    QGraphicsView* m_view;
    QMap<QString, Penguin*> m_penguins;
    QMap<QString, QList<Snowball*>> m_snowballs;
    ClientRequests* m_requests;
    QTimer* m_gameStateTimer;
    Map* m_map;

    QMap<QPair<int, int>, QGraphicsItem*> m_powerUps;

private slots:
    void onGameStateUpdated();
    void onMapReceived(const std::vector<std::vector<int>>& mapData, std::unordered_map<int, std::string>& cellTypes);
};
