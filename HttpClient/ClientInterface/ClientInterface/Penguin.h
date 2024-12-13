#ifndef PENGUIN_H
#define PENGUIN_H

#include <QGraphicsPixmapItem>
#include <QObject>
#include <QKeyEvent>
#include "ClientRequests.h"
#include "ClientState.h"

class Penguin : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT
public:
    Penguin(ClientRequests* requests, QGraphicsPixmapItem* parent=nullptr);
    void resizePenguin(int width, int height);
    void keyPressEvent(QKeyEvent* event);
private:
    ClientRequests* m_clientRequests;
};

#endif
