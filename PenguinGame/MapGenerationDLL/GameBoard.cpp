#include "GameBoard.h"
#include <iostream>
#include <utility>
#include <cstdlib>
#include <cmath>
#include <algorithm>
#include <queue>
#include <ctime>
#include <stdexcept>
#include <ranges>

namespace MapGen {

    uint32_t GameBoard::GetRows() const { return m_rows; }
    uint32_t GameBoard::GetCols() const { return m_cols; }
    std::vector<std::vector<uint32_t>>& GameBoard::GetBoard() { return m_board; }
    const std::vector<std::vector<uint32_t>>& GameBoard::GetBoard() const { return m_board; }

    GameBoard::GameBoard(uint32_t rows, uint32_t cols)
        : m_rows(rows), m_cols(cols), m_board(rows, std::vector<uint32_t>(cols, 0)) {
        std::random_device rd;
        m_rng.seed(rd());
    }

    GameBoard::GameBoard(GameBoard&& other) noexcept
        : m_rows(other.m_rows),
        m_cols(other.m_cols),
        m_board(std::move(other.m_board)),
        m_cellDefinitions(std::move(other.m_cellDefinitions)) {
        other.m_rows = 0;
        other.m_cols = 0;
    }

    GameBoard& GameBoard::operator=(GameBoard&& other) noexcept {
        if (this != &other) 
        {
            m_rows = other.m_rows;
            m_cols = other.m_cols;
            m_board = std::move(other.m_board);
            m_cellDefinitions = std::move(other.m_cellDefinitions);

            other.m_rows = 0;
            other.m_cols = 0;
        }
        return *this;
    }
    /*
    void GameBoard::AddCellType(int id, const CellTypeDefinition& definition) {
        m_cellDefinitions[id] = definition;
    }
    */

    void GameBoard::AddCellType(uint8_t id, CellTypeDefinition definition) {
        m_cellDefinitions.emplace(static_cast<unsigned int>(id), std::move(definition));
    }

    bool GameBoard::ShouldSpawn(const CellTypeDefinition& definition) const {
        return (m_rng() % 100) < definition.spawnChance;
    }

    constexpr uint32_t SquaredDistance(uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2) {
        return static_cast<uint64_t>(x1 - x2) * (x1 - x2) + static_cast<uint64_t>(y1 - y2) * (y1 - y2);
    }


    void GameBoard::InitializeBoard() {
        ReserveCorners();
        CreateRandomPaths();

        // 1. Plasarea grupurilor de ziduri (indestructibile/destructibile)
        for (const auto& [id, definition] : m_cellDefinitions) {
            if (id == 1 || id == 2) {
                uint8_t baseGroupCount = (id == 1) ? 10 : 6; // Mai puține grupuri pentru tipul 2
                uint8_t additionalGroups = (id == 1) ? 8 : 5; // Mai puține variații pentru tipul 2
                uint8_t groupCount = baseGroupCount + (m_rng() % additionalGroups);

                for (uint8_t g = 0; g < groupCount; ++g) {
                    bool placed = false;

                    // Generate random groups with a maximum of 10 attempts
                    for (int attempt = 0; attempt < 10 && !placed; ++attempt) {
                        uint32_t x = m_rng() % m_rows;    // Random între 0 și m_rows - 1
                        uint32_t y = m_rng() % m_cols;    // Random între 0 și m_cols - 1
                        uint32_t width = 2 + (m_rng() % 2);
                        uint32_t height = 2 + (m_rng() % 2); // Random între 2 și 4

                        if (CanPlaceGroup(id, x, y, width, height)) {
                            GenerateRectangleGroup(id, x, y, width, height);
                            placed = true;
                        }
                    }

                    if (!placed) {
                        std::cout << "Warning: Unable to place group for type " << id << " after 10 attempts.\n";
                    }
                }
            }
        }

        // 2. Plasarea bombelor
        int bombCount = 0;
        std::vector<std::pair<int, int>> bombPositions;

        auto allCells = std::views::iota(0U, m_rows) | std::views::transform([this](uint32_t i) {
            return std::views::iota(0U, m_cols) | std::views::transform([i](uint32_t j) {
                return std::pair(i, j);
                });
            }) | std::views::join;

        for (const auto& [i, j] : allCells | std::views::take_while([&](const auto&) { return bombCount < 3; })) {
            if (m_cellDefinitions.count(3) > 0) {
                const auto& bombDefinition = m_cellDefinitions.at(3);

                if (bombDefinition.customRule && bombDefinition.customRule(i, j, m_board)) {
                    bool validPosition = std::ranges::none_of(bombPositions, [&](const std::pair<int, int>& bomb) {
                        return SquaredDistance(i, j, bomb.first, bomb.second) < 10 * 10;
                        });

                    if (validPosition) {
                        m_board[i][j] = 3;
                        bombPositions.emplace_back(i, j);
                        ++bombCount;
                    }
                }
            }
        }

        std::cout << "Number of bombs placed: " << bombCount << "\n";
    }



    void GameBoard::PrintBoard() const {
        for (const auto& row : m_board) {
            for (auto cell : row) {
                std::cout << cell << " ";
            }
            std::cout << "\n";
        }
    }

    GameBoard::~GameBoard() {}


    void GameBoard::GenerateRectangleGroup(uint8_t id, uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
        for (uint32_t i = 0; i < width; ++i) {
            for (uint32_t j = 0; j < height; ++j) {
                uint32_t newX = x + i;
                uint32_t newY = y + j;

                if (newX >= 0 && newX < m_rows && newY >= 0 && newY < m_cols) {
                    m_board[newX][newY] = id;
                }
            }
        }
    }

    bool GameBoard::CanPlaceGroup(uint8_t id, uint32_t x, uint32_t y, uint32_t width, uint32_t height) const noexcept {
        // Coordonatele colțurilor rezervate
        std::vector<std::pair<uint32_t, uint32_t>> reservedCorners = {
            {0, 0},
            {0, m_cols - 1},
            {m_rows - 1, 0},
            {m_rows - 1, m_cols - 1}
        };

        auto isNearReservedCorner = [&](uint32_t cx, uint32_t cy) {
            constexpr uint32_t cornerBuffer = 2; // Distanța minimă față de colțuri
            for (const auto& corner : reservedCorners) {
                if (SquaredDistance(cx, cy, corner.first, corner.second) <= cornerBuffer * cornerBuffer) {
                    return true;
                }
            }
            return false;
            };

        // Verificare distanță față de alte grupuri
        auto isFarFromOtherGroups = [&](uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
            constexpr uint32_t minDistance = 2; // Distanța minimă față de alte grupuri
            for (const auto& group : m_placedGroups) {
                uint32_t gx, gy, gwidth, gheight;
                std::tie(gx, gy, gwidth, gheight) = group;

                // Verificăm suprapunerea extinsă cu buffer
                if (!(x >= gx + gwidth + minDistance || gx >= x + width + minDistance || // Orizontal
                    y >= gy + gheight + minDistance || gy >= y + height + minDistance)) { // Vertical
                    return false;
                }
            }
            return true;
            };

        // Verifică fiecare celulă a grupului
        for (uint32_t i = 0; i < width; ++i) {
            for (uint32_t j = 0; j < height; ++j) {
                uint32_t newX = x + i;
                uint32_t newY = y + j;

                // Verifică limitele tablei
                if (newX >= m_rows || newY >= m_cols) {
                    return false;
                }

                // Verifică dacă zona este liberă
                if (m_board[newX][newY] != 0) {
                    return false;
                }

                // Verifică dacă celula este prea aproape de colțuri rezervate
                if (isNearReservedCorner(newX, newY)) {
                    return false;
                }
            }
        }

        // Verifică dacă grupul este suficient de departe de alte grupuri
        if (!isFarFromOtherGroups(x, y, width, height)) {
            return false;
        }

        return true;
    }




    void GameBoard::ReserveCorners() {
        m_board[0][0] = 0;
        m_board[0][m_cols - 1] = 0;
        m_board[m_rows - 1][0] = 0;
        m_board[m_rows - 1][m_cols - 1] = 0;
    }


    void GameBoard::CreateRandomPaths() {
        std::vector<std::pair<int, int>> corners = {
            {0, 0},
            {0, m_cols - 1},
            {m_rows - 1, 0},
            {m_rows - 1, m_cols - 1}
        };

        for (size_t i = 0; i < corners.size(); ++i) {
            auto start = corners[i];
            auto end = corners[(i + 1) % corners.size()];
            CreateRandomPath(start, end);
        }
    }

    bool GameBoard::IsPathPossible(const std::pair<uint32_t, uint32_t>& start,const  std::pair<uint32_t, uint32_t>& end) const {
        // use a BFS approach
        std::queue<std::pair<uint32_t, uint32_t>> toVisit;
        std::vector<std::vector<bool>> visited(m_rows, std::vector<bool>(m_cols, false));

        toVisit.push(start);
        visited[start.first][start.second] = true;

        std::vector<std::pair<int, int>> directions = { {-1, 0}, {1, 0}, {0, -1}, {0, 1} };

        while (!toVisit.empty()) {
            auto [x, y] = toVisit.front();
            toVisit.pop();

            if (x == end.first && y == end.second) {
                return true;
            }

            for (const auto& dir : directions) {
                int newX = x + dir.first;
                int newY = y + dir.second;

                if (newX >= 0 && newX < m_rows && newY >= 0 && newY < m_cols &&
                    !visited[newX][newY] && m_board[newX][newY] == 0) {
                    visited[newX][newY] = true;
                    toVisit.push({ newX, newY });
                }
            }
        }

        return false;
    }

    void GameBoard::CreateRandomPath(const std::pair<uint32_t, uint32_t>& start, const std::pair<uint32_t, uint32_t>& end) {
        if (!IsPathPossible(start, end)) {
            std::cout << "Path not possible, creating a free path.\n";
            ForcePath(start, end);
        }

        uint32_t x = start.first;
        uint32_t y = start.second;

        m_board[x][y] = 0;

        std::vector<std::pair<uint32_t, uint32_t>> directions = { {-1, 0}, {1, 0}, {0, -1}, {0, 1} };
        std::random_device rd;
        std::mt19937 gen(rd());

        while (x != end.first || y != end.second) {
            std::shuffle(directions.begin(), directions.end(), m_rng);
            bool moved = false;

            for (const auto& dir : directions) {
                uint32_t newX = x + dir.first;
                uint32_t newY = y + dir.second;

                if (newX >= 0 && newX < m_rows && newY >= 0 && newY < m_cols &&
                    m_board[newX][newY] == 0) {
                    x = newX;
                    y = newY;
                    m_board[x][y] = 0;
                    moved = true;
                    break;
                }
            }

            if (!moved) {
                if (x != end.first) {
                    x += (x < end.first) ? 1 : -1;
                }
                else if (y != end.second) {
                    y += (y < end.second) ? 1 : -1;
                }
                m_board[x][y] = 0;
            }
        }
    }

    void GameBoard::ForcePath(std::pair<uint32_t, uint32_t> start, std::pair<uint32_t, uint32_t> end) {
        uint32_t x = start.first;
        uint32_t y = start.second;

        m_board[x][y] = 0;

        while (x != end.first || y != end.second) {
            if (x != end.first) {
                x += (x < end.first) ? 1 : -1;
            }
            else if (y != end.second) {
                y += (y < end.second) ? 1 : -1;
            }

            m_board[x][y] = 0;
        }
    }
}

std::vector<std::pair<uint32_t, uint32_t>> MapGen::GameBoard::GetStartingPositions() const noexcept {
    return {
        {0, 0},
        {0, static_cast<int>(m_cols) - 1},
        {static_cast<int>(m_rows) - 1, static_cast<int>(m_cols) - 1},
        {static_cast<int>(m_rows) - 1, 0}
    };
}

