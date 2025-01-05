#include "Snowball.h"
#include <QGraphicsScene>

Snowball::Snowball(int startX, int startY, const QString& direction, float speed, QGraphicsItem* parent)
    : QObject(), QGraphicsEllipseItem(parent), m_speed(speed), m_direction(direction) {

    const int cellSize = Map::getCellSize();
    const int centerOffset = cellSize / 2;

    // Setăm poziția inițială în centrul celulei
    setRect(0, 0, 10, 10); // Dimensiunile proiectilului
    setBrush(Qt::blue);
    setPos(startX + centerOffset - rect().width() / 2,
        startY + centerOffset - rect().height() / 2);

    qDebug() << "Snowball created at:" << x() << y() << "Direction:" << direction << "Speed:" << speed;

    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &Snowball::move);
    m_timer->start(50); // Intervalul de mișcare
}




Snowball::~Snowball() {
    delete m_timer;
}

void Snowball::move() {
    const int cellSize = Map::getCellSize();
    const float speedMultiplier = cellSize / 4.0f; // Deplasare în trepte mici pentru fluiditate
    QPointF currentPosition = pos();

    if (m_direction == "up") {
        setY(currentPosition.y() - m_speed * speedMultiplier);
    }
    else if (m_direction == "down") {
        setY(currentPosition.y() + m_speed * speedMultiplier);
    }
    else if (m_direction == "left") {
        setX(currentPosition.x() - m_speed * speedMultiplier);
    }
    else if (m_direction == "right") {
        setX(currentPosition.x() + m_speed * speedMultiplier);
    }

    // Verificăm dacă boundingRect al Snowball iese din limitele scenei
    if (scene()) {
        const QRectF mapBounds = scene()->sceneRect(); // Obține limitele scenei
        QRectF snowballBounds = mapToScene(boundingRect()).boundingRect(); // Translatăm boundingRect în coordonate ale scenei

        if (!mapBounds.contains(snowballBounds)) {
            qDebug() << "Snowball exited map bounds and will be removed.";
            scene()->removeItem(this);
            delete this;
        }
    }
    else {
        qDebug() << "Snowball is not in any scene. It will be deleted.";
        delete this;
    }
}
