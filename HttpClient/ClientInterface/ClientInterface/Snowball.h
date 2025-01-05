#pragma once

#include <QGraphicsEllipseItem>
#include <QObject>
#include <QTimer>
#include "Map.h"

class Snowball : public QObject, public QGraphicsEllipseItem {
    Q_OBJECT
private:
    float m_speed;
    QString m_direction;
    QTimer* m_timer;

private slots:
    void move();

public:
    Snowball(int startX, int startY, const QString& direction, float speed, QGraphicsItem* parent = nullptr);
    ~Snowball();
};
