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

private:
    QGraphicsScene* m_scene;
    QGraphicsView* m_view;
    QMap<QString, Penguin*> m_penguins;
    QMap<QString, QList<Snowball*>> m_snowballs;
    ClientRequests* m_requests;
    Map* m_map;

private slots:
    void onGameStateUpdated();
    void onMapReceived(const std::vector<std::vector<int>>& mapData, std::unordered_map<int, std::string>& cellTypes);
    void debugPrintPenguins() const;
    void onGameEventsReceived(const QVector<QPair<QString, QPair<int, int>>>& events);
    //void onSnowballFired(int startX, int startY, const QString& direction, float speed);

public:
    explicit GamePage(ClientRequests* requests, QWidget* parent = nullptr);
    ~GamePage();
};
