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

    // Method to generate a random map
    void generateRandomMap(uint16_t wallChance, uint16_t bombChance);

    // Method to create free paths on the map
    void createPaths();

    // Method to set a specific cell
    void setCell(uint32_t x, uint32_t y, GenericElementType type, WallType wallType = WallType::Indestructible);

    // Method to get a specific cell
    Cell getCell(uint32_t x, uint32_t y) const;

    // Getter for the grid
    const std::vector<std::vector<Cell>>& getGrid() const;

    // Getters for map dimensions
    uint32_t getRows() const;
    uint32_t getCols() const;

private:
    // Helper method to create a path between two points
    void createPath(std::pair<int, int> start, std::pair<int, int> end);
};

// Exported functions for DLL usage
extern "C" __declspec(dllexport) Map GenerateRandomMap(uint32_t rows, uint32_t cols, uint16_t wallChance, uint16_t bombChance);
extern "C" __declspec(dllexport) void CreatePaths(Map& gameMap);
extern "C" __declspec(dllexport) void SetCell(Map& gameMap, uint32_t x, uint32_t y, Cell cellType);
