#include "Snowball.h"
#include <QGraphicsScene>

Snowball::Snowball(int startX, int startY, QGraphicsItem* parent)
    : QObject(), QGraphicsEllipseItem(parent)
{

    const int cellSize = Map::getCellSize();
    const int centerOffset = cellSize / 2;

    setRect(0, 0, 10, 10);
    setBrush(Qt::blue);

    setPos(startX + centerOffset - rect().width() / 2,
        startY + centerOffset - rect().height() / 2);
}

void Snowball::updatePosition(int x, int y) {
    const int cellSize = Map::getCellSize();
    const int centerOffset = cellSize / 2;

    setPos(x * cellSize + centerOffset - rect().width() / 2,
        y * cellSize + centerOffset - rect().height() / 2);
}
