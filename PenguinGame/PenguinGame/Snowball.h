#pragma once
#include <tuple>
#include <string>
#include <chrono>
#include "GameBoard.h"

class Snowball
{
public:
    Snowball(const std::pair<int, int>& startPosition, const std::string& launchDirection, float launchSpeed = 0.25f);
    ~Snowball() = default;

    void UpdatePosition(GameBoard& gameBoard, const std::vector<Snowball>& activeSnowballs);
    bool CheckCollision(GameBoard& gameBoard, const std::vector<Snowball>& activeSnowballs);
    bool IsActive() const { return m_active; }
    void Deactivate() { m_active = false; }
    std::pair<int, int> GetPosition() const { return m_position; }

private:
    std::pair<int, int> GetNextPosition() const;

private:
    std::pair<int, int> m_position;
    std::string m_direction;
    float m_speed;
    bool m_active;
    std::chrono::steady_clock::time_point m_lastUpdate; // pentru a controla intervalele de timp intre miscarile glontului
    // retine cand glontul si a schimbat pozitia ultima oara
};

