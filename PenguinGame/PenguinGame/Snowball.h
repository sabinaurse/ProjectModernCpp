#pragma once
#include <tuple>
#include <string>
#include <chrono>
#include "GameBoard.h"

class Snowball
{
public:
    Snowball(std::tuple<int, int> start_position, const std::string& launch_direction, float launch_speed = 0.25);
    ~Snowball() = default;

    void updatePosition(GameBoard& gameBoard);
    bool checkCollision(GameBoard& gameBoard);
    bool isActive() const;
    void deactivate();

private:
    std::tuple<int, int> getNextPosition();

private:
    std::tuple<int, int> m_position;
    std::string m_direction;
    float m_speed;
    bool m_active;
    std::chrono::steady_clock::time_point m_lastUpdate; // pentru a controla intervalele de timp intre miscarile glontului
    // retine cand glontul si a schimbat pozitia ultima oara
};

