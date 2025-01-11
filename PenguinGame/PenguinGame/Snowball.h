#pragma once
#include <tuple>
#include <string>
#include <chrono>
#include "BoardManager.h"

class Snowball
{
public:
    Snowball(const std::pair<int, int>& startPosition, const std::string& launchDirection);
    ~Snowball() = default;

    void UpdatePosition(const MapGen::GameBoard& gameBoard);
    bool IsActive() const { return m_active; }
    void Deactivate() { m_active = false; }
    std::pair<int, int> GetPosition() const { return m_position; }
    float GetSpeed() const { return m_bulletSpeed; };
    std::string GetDirection() const { return m_direction; };

private:
    std::pair<int, int> GetNextPosition() const;

private:
    std::pair<int, int> m_position;
    std::string m_direction;
    float m_bulletSpeed{ 5.0f };
    bool m_active;
    std::chrono::steady_clock::time_point m_lastUpdate; // controleaza intervalele de miscare
};

