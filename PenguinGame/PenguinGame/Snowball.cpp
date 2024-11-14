#include "Snowball.h"

Snowball::Snowball(const std::tuple<int, int>& startPosition, const std::string& launchDirection, float launchSpeed)
    : m_position(startPosition), m_direction(launchDirection), m_speed(launchSpeed),
    m_active(true), m_lastUpdate(std::chrono::steady_clock::now()) {}

void Snowball::UpdatePosition(GameBoard& gameBoard, const std::vector<Snowball>& activeSnowballs)
{
    // verificam daca a trecut suficient timp pentru a misca glontul
    auto currentTime = std::chrono::steady_clock::now();
    std::chrono::duration<float> elapsedTime = currentTime - m_lastUpdate;

    if (elapsedTime.count() >= 1.0f / m_speed) // folosim viteza pentru a calcula timpul necesar miscarii
    {
        m_position = getNextPosition();
        m_lastUpdate = currentTime;

        if (CheckCollision(gameBoard, activeSnowballs))
            Deactivate();
    }
}

bool Snowball::CheckCollision(GameBoard& gameBoard, const std::vector<Snowball>& activeSnowballs) {
    int x = std::get<0>(m_position);
    int y = std::get<1>(m_position);

    if (gameBoard.isWithinBounds(x, y)) {
        Cell cell = gameBoard.getCell(x, y);

        switch (cell) {
        case Cell::Destructible_Wall:
            gameBoard.destroyCell(x, y);
            return true;

        case Cell::Hidden_Bomb:
            gameBoard.triggerExplosion(x, y);
            return true;

        case Cell::Indestructible_Wall:
            return true;

        case Cell::Empty:
        default:
            break;
        }
    }

    // verificam coliziunea cu alti snowballs
    // eventual mutata in game??
    for (const auto& other : activeSnowballs) {
        if (this != &other && other.IsActive() && m_position == other.GetPosition()) {
            Deactivate();
            const_cast<Snowball&>(other).Deactivate();
            std::cout << "Two snowballs collided and were deactivated" << std::endl; // debugging
            return true;
        }
    }

    return false; // nicio coliziune
}



std::tuple<int, int> Snowball::getNextPosition() const
{
    int x = std::get<0>(m_position);
    int y = std::get<1>(m_position);

    if (m_direction == "up") --y;
    else if (m_direction == "down") ++y;
    else if (m_direction == "left") --x;
    else if (m_direction == "right") ++x;

    return { x,y };
}



