#pragma once
#include <tuple>
#include <string>

class Snowball
{
private:
    std::tuple<int, int> m_position;
    std::string m_direction;
    float m_speed;
    bool m_active;
public:
    Snowball(std::tuple<int, int> start_position, std::string launch_direction, float launch_speed = 0.25);
    ~Snowball() = default;
};

