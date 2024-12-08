#pragma once

#include <vector>
#include <cstdint>
#include <utility>
#include <cstdlib>
#include <ctime>

// Enum for cell types
enum class WallType {
    Indestructible, // Walls that cannot be destroyed
    Destructible    // Walls that can be destroyed
};

enum class GenericElementType {
    Wall,         // Wall cell
    Player,       // Player cell
    Enemy,        // Enemy cell
    PowerUp,      // Power-up cell
    Bomb,
    Obstacle,     // Obstacle cell
    Collectible,  // Collectible item cell
    Empty         // Empty cell
};

// Structure representing a cell on the map
struct Cell {
    GenericElementType type; // Type of the element in the cell
    WallType wallType;       // Type of wall if the cell is a wall
};

// Map class for generating and managing the game map
class Map {
private:
    std::vector<std::vector<Cell>> grid; // 2D grid representing the map
    uint32_t rows; // Number of rows in the map
    uint32_t cols; // Number of columns in the map

public:
    // Constructor
    Map(uint32_t rows, uint32_t cols);

    // Generate a random map
    void generateRandomMap();

    // Check if a bomb can be placed at a position
    bool IsFarEnoughFromOtherBombs(uint32_t x, uint32_t y);

    // Methods to determine if specific elements should appear
    bool WillDestructibleWallAppear();
    bool WillIndestructibleWallAppear();
    bool WillBombAppear(uint32_t x, uint32_t y);
    bool WillCellBeEmptyAppear();

    // Set a specific cell
    void setCell(uint32_t x, uint32_t y, GenericElementType type, WallType wallType = WallType::Indestructible);

    // Get a specific cell
    Cell getCell(uint32_t x, uint32_t y) const;

    // Getter for grid
    const std::vector<std::vector<Cell>>& getGrid() const;

    // Getters for dimensions
    uint32_t getRows() const;
    uint32_t getCols() const;

private:
    // Helper method to create a path between two points
    void createPath(std::pair<int, int> start, std::pair<int, int> end);
};
