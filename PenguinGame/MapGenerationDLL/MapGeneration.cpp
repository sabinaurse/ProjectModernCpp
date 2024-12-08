#include "MapGeneration.h"
#include <cmath> // For std::abs

// Constructor
Map::Map(uint32_t rows, uint32_t cols)
    : rows(rows), cols(cols), grid(rows, std::vector<Cell>(cols, { GenericElementType::Empty, WallType::Indestructible })) {
}

// Generate a random map
void Map::generateRandomMap() {
    std::srand(static_cast<unsigned int>(std::time(0))); // Seed for random numbers

    for (uint32_t i = 0; i < rows; ++i) {
        for (uint32_t j = 0; j < cols; ++j) {
            if (WillDestructibleWallAppear()) {
                grid[i][j] = { GenericElementType::Wall, WallType::Destructible };
            }
            else if (WillIndestructibleWallAppear()) {
                grid[i][j] = { GenericElementType::Wall, WallType::Indestructible };
            }
            else if (WillBombAppear(i, j)) {
                grid[i][j] = { GenericElementType::Bomb, WallType::Indestructible };
            }
            else if (WillCellBeEmptyAppear()) {
                grid[i][j] = { GenericElementType::Empty, WallType::Indestructible };
            }
            else {
                grid[i][j] = { GenericElementType::Obstacle, WallType::Indestructible };
            }
        }
    }
}

// Check if a bomb is far enough from other bombs
bool Map::IsFarEnoughFromOtherBombs(uint32_t x, uint32_t y) {
    const int minDistance = 2; // Minimum safe distance

    for (int i = -minDistance; i <= minDistance; ++i) {
        for (int j = -minDistance; j <= minDistance; ++j) {
            int nx = x + i;
            int ny = y + j;

            // Check if the neighboring cell is within bounds
            if (nx >= 0 && ny >= 0 && nx < static_cast<int>(rows) && ny < static_cast<int>(cols)) {
                if (grid[nx][ny].type == GenericElementType::Bomb) {
                    return false; // Another bomb is too close
                }
            }
        }
    }
    return true; // Safe to place a bomb
}

// Determine if a destructible wall should appear
bool Map::WillDestructibleWallAppear() {
    const int probability = 30; // 30% chance
    return (std::rand() % 100) < probability;
}

// Determine if an indestructible wall should appear
bool Map::WillIndestructibleWallAppear() {
    const int probability = 10; // 10% chance
    return (std::rand() % 100) < probability;
}

// Determine if a bomb should appear
bool Map::WillBombAppear(uint32_t x, uint32_t y) {
    const int probability = 20; // 20% chance
    return (std::rand() % 100) < probability && IsFarEnoughFromOtherBombs(x, y);
}

// Determine if a cell should be empty
bool Map::WillCellBeEmptyAppear() {
    const int probability = 40; // 40% chance
    return (std::rand() % 100) < probability;
}

// Set a specific cell
void Map::setCell(uint32_t x, uint32_t y, GenericElementType type, WallType wallType) {
    if (x < rows && y < cols) {
        grid[x][y] = { type, wallType };
    }
}

// Get a specific cell
Cell Map::getCell(uint32_t x, uint32_t y) const {
    if (x < rows && y < cols) {
        return grid[x][y];
    }
    return { GenericElementType::Empty, WallType::Indestructible };
}

// Getter for grid
const std::vector<std::vector<Cell>>& Map::getGrid() const {
    return grid;
}

// Getter for rows
uint32_t Map::getRows() const {
    return rows;
}

// Getter for columns
uint32_t Map::getCols() const {
    return cols;
}

// Create a path between two points
void Map::createPath(std::pair<int, int> start, std::pair<int, int> end) {
    int x = start.first, y = start.second;
    int targetX = end.first, targetY = end.second;

    while (x != targetX || y != targetY) {
        grid[x][y] = { GenericElementType::Empty, WallType::Indestructible };
        if (x != targetX) {
            x += (x < targetX) ? 1 : -1;
        }
        else if (y != targetY) {
            y += (y < targetY) ? 1 : -1;
        }
    }
    grid[x][y] = { GenericElementType::Empty, WallType::Indestructible };
}
