#include "Snowball.h"

Snowball::Snowball(std::tuple<int, int> start_position, std::string launch_direction, float launch_speed)
	: m_position(start_position), m_direction(launch_direction), m_speed(launch_speed), m_active(true)
{
}
