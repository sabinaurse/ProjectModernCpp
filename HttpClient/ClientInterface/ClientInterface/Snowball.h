#pragma once

#include <QGraphicsEllipseItem>
#include <QObject>
#include <QTimer>
#include "Map.h"

class Snowball : public QObject, public QGraphicsEllipseItem {
    Q_OBJECT

public:
    Snowball(int startX, int startY,QGraphicsItem* parent = nullptr);
    void updatePosition(int x, int y);
    ~Snowball() = default;
};
