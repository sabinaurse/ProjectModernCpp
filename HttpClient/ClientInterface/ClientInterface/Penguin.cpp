﻿#include "Penguin.h"


Penguin::Penguin(ClientRequests* requests, QGraphicsPixmapItem* parent)
    : QObject(nullptr), QGraphicsPixmapItem(parent), m_clientRequests(requests) {

    int cellSize = Map::getCellSize();

    m_sprites["front"] = resizePixmap(QPixmap("sprites/Penguin_front.png"), cellSize, cellSize);
    m_sprites["back"] = resizePixmap(QPixmap("sprites/Penguin_back.png"), cellSize, cellSize);
    m_sprites["left"] = resizePixmap(QPixmap("sprites/Penguin_left.png"), cellSize, cellSize);
    m_sprites["right"] = resizePixmap(QPixmap("sprites/Penguin_right.png"), cellSize, cellSize);

    setPixmap(m_sprites["front"]);
    setFlag(QGraphicsItem::ItemIsFocusable);
    setFocus();
}

QPixmap Penguin::resizePixmap(const QPixmap& pixmap, int width, int height) {
    return pixmap.scaled(width, height, Qt::KeepAspectRatio, Qt::FastTransformation);
}

void Penguin::keyPressEvent(QKeyEvent* event) {
    QString playerName = ClientState::instance().GetCurrentPlayer();
    if (playerName.isEmpty()) return;

    QString direction;
    if (event->key() == Qt::Key_W) {
        direction = "W";
        setPixmap(m_sprites["back"]);
    }
    else if (event->key() == Qt::Key_S) {
        direction = "S";
        setPixmap(m_sprites["front"]);
    }
    else if (event->key() == Qt::Key_A) {
        direction = "A";
        setPixmap(m_sprites["left"]);
    }
    else if (event->key() == Qt::Key_D) {
        direction = "D";
        setPixmap(m_sprites["right"]);
    }
    else if (event->key() == Qt::Key_F) {
        m_clientRequests->Fire(playerName);
        return;
    }

    if (!direction.isEmpty()) {
        m_clientRequests->MovePlayer(playerName, direction);
    }
}


