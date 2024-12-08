#include "MapGeneration.h"

// Constructor
// Initializes the map with the specified number of rows and columns.
// All cells are set to be empty by default with indestructible walls.
Map::Map(uint32_t rows, uint32_t cols)
    : rows(rows), cols(cols), grid(rows, std::vector<Cell>(cols, { GenericElementType::Empty, WallType::Indestructible })) {
}

// Generate a random map
// Randomly populates the grid with walls and obstacles based on the provided chances.
void Map::generateRandomMap(uint16_t wallChance, uint16_t bombChance) {
    std::srand(static_cast<unsigned int>(std::time(0))); // Seed for random number generation

    for (uint32_t i = 0; i < rows; ++i) {
        for (uint32_t j = 0; j < cols; ++j) {
            int randValue = std::rand() % 100;

            if (randValue < wallChance) {
                // Randomly assign a destructible or indestructible wall
                grid[i][j] = { GenericElementType::Wall, (rand() % 2 == 0) ? WallType::Indestructible : WallType::Destructible };
            }
            else if (randValue < wallChance + bombChance) {
                // Set the cell as an obstacle
                grid[i][j] = { GenericElementType::Obstacle, WallType::Indestructible };
            }
        }
    }
}

// Create free paths
// Ensures that there are free paths connecting the four corners of the map.
void Map::createPaths() {
    std::vector<std::pair<int, int>> corners = {
        {0, 0}, {0, cols - 1}, {rows - 1, cols - 1}, {rows - 1, 0}
    };

    for (size_t i = 0; i < corners.size(); ++i) {
        auto start = corners[i];
        auto end = corners[(i + 1) % corners.size()];
        createPath(start, end);
    }
}

// Set a specific cell
// Updates the cell at the given (x, y) position with the specified type and wall type.
void Map::setCell(uint32_t x, uint32_t y, GenericElementType type, WallType wallType) {
    if (x < rows && y < cols) {
        grid[x][y] = { type, wallType };
    }
}

// Get a specific cell
// Returns the cell at the given (x, y) position. If the position is out of bounds, returns an empty cell.
Cell Map::getCell(uint32_t x, uint32_t y) const {
    if (x < rows && y < cols) {
        return grid[x][y];
    }
    return { GenericElementType::Empty, WallType::Indestructible };
}

// Getter for the grid
// Returns the entire grid as a 2D vector of cells.
const std::vector<std::vector<Cell>>& Map::getGrid() const {
    return grid;
}

// Getter for the number of rows
uint32_t Map::getRows() const {
    return rows;
}

// Getter for the number of columns
uint32_t Map::getCols() const {
    return cols;
}

// Create a path between two points
// Sets all cells along the path between the start and end points to be empty.
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

// Exported function: Generate a random map
// Creates a new map with random walls and obstacles based on the given chances.
extern "C" __declspec(dllexport) Map GenerateRandomMap(uint32_t rows, uint32_t cols, uint16_t wallChance, uint16_t bombChance) {
    Map gameMap(rows, cols);
    gameMap.generateRandomMap(wallChance, bombChance);
    return gameMap;
}

// Exported function: Create paths
// Ensures free paths are created on the given map.
extern "C" __declspec(dllexport) void CreatePaths(Map& gameMap) {
    gameMap.createPaths();
}

// Exported function: Set a specific cell
// Updates the cell in the map at the specified position with the given type and wall type.
extern "C" __declspec(dllexport) void SetCell(Map& gameMap, uint32_t x, uint32_t y, Cell cellType) {
    gameMap.setCell(x, y, cellType.type, cellType.wallType);
}
