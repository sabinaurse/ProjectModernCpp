#include "Snowball.h"

Snowball::Snowball(const std::pair<int, int>& startPosition, Direction launchDirection, float bulletSpeed)
	: m_position{ startPosition },
	m_direction{ launchDirection },
	m_bulletSpeed{ bulletSpeed },
	m_active{ true },
	m_lastUpdate{ std::chrono::steady_clock::now() }
{
}

void Snowball::UpdatePosition(const MapGen::GameBoard& gameBoard)
{
	auto currentTime = std::chrono::steady_clock::now();
	std::chrono::duration<float> elapsedTime = currentTime - m_lastUpdate;


	if (elapsedTime.count() >= 1.0f / m_bulletSpeed)
	{
		auto nextPosition = GetNextPosition();
		auto& board = gameBoard.GetBoard();

		if (nextPosition.first >= 0 && nextPosition.first < static_cast<int>(board.size()) &&
			nextPosition.second >= 0 && nextPosition.second < static_cast<int>(board[0].size()))
		{
			int cellType = board[nextPosition.first][nextPosition.second];
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

std::pair<int, int> Snowball::GetNextPosition() const {
	int x = m_position.first;
	int y = m_position.second;

	switch (m_direction) {
	case Direction::Up:    --y; break;
	case Direction::Down:  ++y; break;
	case Direction::Left:  --x; break;
	case Direction::Right: ++x; break;
	}
	return { x, y };
}