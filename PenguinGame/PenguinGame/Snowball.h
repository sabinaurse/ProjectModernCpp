#pragma once
#include <tuple>
#include <string>
#include <chrono>
#include <ostream>
#include "BoardManager.h"

enum class Direction
{
    Up,
    Down,
    Left,
    Right
};

inline std::string DirectionToString(Direction direction) {
    switch (direction) {
    case Direction::Up:    return "up";
    case Direction::Down:  return "down";
    case Direction::Left:  return "left";
    case Direction::Right: return "right";
    default:               return "unknown";
    }
}

inline std::ostream& operator<<(std::ostream& os, Direction direction) {
    os << DirectionToString(direction);
    return os;
}

class Snowball
{
public:
    Snowball(const std::pair<int, int>& startPosition, Direction launchDirection, float bulletSpeed);
    ~Snowball() = default;

    void UpdatePosition(const MapGen::GameBoard& gameBoard);
    bool IsActive() const { return m_active; }
    void Deactivate() { m_active = false; }

    std::pair<int, int> GetPosition() const { return m_position; }
    Direction GetDirection() const { return m_direction; }
    float GetBulletSpeed() const { return m_bulletSpeed; }

private:
    std::pair<int, int> GetNextPosition() const;

private:
    std::pair<int, int> m_position;
    Direction m_direction;
    bool m_active;
    float m_bulletSpeed;
    std::chrono::steady_clock::time_point m_lastUpdate; 
};
