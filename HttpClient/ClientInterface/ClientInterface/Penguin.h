#pragma once

#include <QGraphicsPixmapItem>
#include <QObject>
#include <QKeyEvent>
#include <QMap>
#include <QPixmap>

#include "ClientRequests.h"
#include "ClientState.h"
#include "Map.h"

class Penguin : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT

public:
    Penguin(ClientRequests* requests, QGraphicsPixmapItem* parent=nullptr);
    QPixmap resizePixmap(const QPixmap& pixmap, int width, int height);

protected:
    void keyPressEvent(QKeyEvent* event) override;

private:
    ClientRequests* m_clientRequests;
    QMap<QString, QPixmap> m_sprites;


};
