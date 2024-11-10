#include "Snowball.h"

Snowball::Snowball(std::tuple<int, int> start_position, const std::string& launch_direction, float launch_speed)
	: m_position(start_position), m_direction(launch_direction), m_speed(launch_speed), m_active(true),
		m_lastUpdate(std::chrono::steady_clock::now())
{
}

void Snowball::updatePosition(GameBoard& gameBoard)
{
	// verificam daca a trecut suficient timp pentru a misca glontul
	auto currentTime = std::chrono::steady_clock::now();
	std::chrono::duration<float> elapsedTime = currentTime - m_lastUpdate;

	if (elapsedTime.count() >= 4.0f)
	{
		m_position = getNextPosition();
		m_lastUpdate = currentTime;

		if (checkCollision(gameBoard))
			deactivate();
	}
}

bool Snowball::checkCollision(GameBoard& gameBoard)
{
	int x = std::get<0>(m_position);
	int y = std::get<1>(m_position);

	if (gameBoard.isWithinBounds(x, y))
	{
		Cell cell = gameBoard.getCell(x, y);

		if (cell == Cell::Destructible_Wall)
		{
			gameBoard.destroyCell(x, y);
			return true;
		}

		if (cell == Cell::Indestructible_Wall || cell == Cell::Bomb)
			return true;

		return false; // nu am avut nicio coliziune
	}
}

bool Snowball::isActive() const
{
	return m_active;
}

void Snowball::deactivate()
{
	m_active = false;
}

void Snowball::doubleSpeed()
{
	m_speed *= 2;
}

std::tuple<int, int> Snowball::getNextPosition()
{
	int x = std::get<0>(m_position);
	int y = std::get<1>(m_position);

	if (m_direction == "up") y = y + 1;
	else if (m_direction == "down") y = y - 1;
	else if (m_direction == "left") x = x - 1;
	else if (m_direction == "right") x = x + 1;

	return { x,y };
}



