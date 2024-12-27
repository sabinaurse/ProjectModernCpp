#include "Snowball.h"

Snowball::Snowball(const std::pair<int, int>& startPosition, const std::string& launchDirection, float launchSpeed)
    : m_position{ startPosition }, m_direction{ launchDirection }, m_speed{ launchSpeed },
    m_active{ true }, m_lastUpdate{ std::chrono::steady_clock::now() } {}


void Snowball::UpdatePosition(MapGen::GameBoard& gameBoard)
{
    auto currentTime = std::chrono::steady_clock::now();
    std::chrono::duration<float> elapsedTime = currentTime - m_lastUpdate;

    if (elapsedTime.count() >= 1.0f / m_speed)
    {
        auto nextPosition = GetNextPosition();
        auto& board = gameBoard.GetBoard();

        if (nextPosition.first >= 0 && nextPosition.first < static_cast<int>(board.size()) &&
            nextPosition.second >= 0 && nextPosition.second < static_cast<int>(board[0].size()))
        {
            int cellType = board[nextPosition.first][nextPosition.second];
            // if cell is empty we update
            if (cellType == 0)
            {
                m_position = nextPosition;
            }
            else 
            {
                Deactivate();
            }
        }
        else
        {
            Deactivate();
        }

        m_lastUpdate = currentTime; 
    }
}

std::pair<int, int> Snowball::GetNextPosition() const
{
    int x = std::get<0>(m_position);
    int y = std::get<1>(m_position);

    if (m_direction == "up") --y;
    else if (m_direction == "down") ++y;
    else if (m_direction == "left") --x;
    else if (m_direction == "right") ++x;

    return std::make_pair(x, y);
}
