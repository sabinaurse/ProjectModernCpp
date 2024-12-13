#include "Penguin.h"

Penguin::Penguin(ClientRequests* requests, QGraphicsPixmapItem* parent)
    : QObject(), QGraphicsPixmapItem(parent), m_clientRequests(requests) {
    setPixmap(QPixmap("C:/Users/Diana/Desktop/sprites/Penguin_front.png"));
    resizePenguin(100, 100);
    setFlag(QGraphicsItem::ItemIsFocusable);
    setFocus();
}

void Penguin::resizePenguin(int width, int height) {
    QPixmap currentPixmap = pixmap(); 
    QPixmap scaledPixmap = currentPixmap.scaled(width, height, Qt::KeepAspectRatio, Qt::FastTransformation);
    setPixmap(scaledPixmap);
}

void Penguin::keyPressEvent(QKeyEvent* event) {
    QString playerName = ClientState::instance().GetCurrentPlayer();

    if (event->key() == Qt::Key_F) {
        if (!playerName.isEmpty()) {
            m_clientRequests->Fire(playerName);
        }
    }
    else {
        QString direction;
        if (event->key() == Qt::Key_W) {
            direction = "W";
            setPixmap(QPixmap("C:/Users/Diana/Desktop/sprites/Penguin_back.png"));
            resizePenguin(100, 100);
        }
        else if (event->key() == Qt::Key_S) {
            direction = "S";
            setPixmap(QPixmap("C:/Users/Diana/Desktop/sprites/Penguin_front.png"));
            resizePenguin(100, 100);
        }
        else if (event->key() == Qt::Key_A) {
            direction = "A";
            setPixmap(QPixmap("C:/Users/Diana/Desktop/sprites/Penguin_left.png"));
            resizePenguin(100, 100);
        }
        else if (event->key() == Qt::Key_D) {
            direction = "D";
            setPixmap(QPixmap("C:/Users/Diana/Desktop/sprites/Penguin_right.png"));
            resizePenguin(100, 100);
        }

        if (!direction.isEmpty() && !playerName.isEmpty()) {
            m_clientRequests->MovePlayer(playerName, direction);
        }
    }
}
