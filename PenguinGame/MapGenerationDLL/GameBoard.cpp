#include "GameBoard.h"
#include <iostream>
#include <utility>
#include <cstdlib>
#include <cmath>
#include <algorithm>
#include <queue>
#include <random>
#include <ctime>
#include <stdexcept>
#include <ranges>

namespace MapGen {

    uint32_t GameBoard::GetRows() const { return m_rows; }
    uint32_t GameBoard::GetCols() const { return m_cols; }
    std::vector<std::vector<int>>& GameBoard::GetBoard() { return m_board; }
    const std::vector<std::vector<int>>& GameBoard::GetBoard() const { return m_board; }

    // de scos srand??- look into it
    GameBoard::GameBoard(uint32_t rows, uint32_t cols)
        : m_rows(rows), m_cols(cols), m_board(rows, std::vector<int>(cols, 0)) {
        std::srand(static_cast<unsigned int>(std::time(nullptr)));
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

    void GameBoard::AddCellType(int id, CellTypeDefinition definition) {
        m_cellDefinitions.emplace(id, std::move(definition));
    }

    bool GameBoard::ShouldSpawn(const CellTypeDefinition& definition) const {
        return std::rand() % 100 < definition.spawnChance;
    }

    inline int SquaredDistance(int x1, int y1, int x2, int y2) {
        return (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2);
    }

    //void GameBoard::InitializeBoard() {
    //    ReserveCorners();
    //    CreateRandomPaths();

    //    for (const auto& [id, definition] : m_cellDefinitions) {
    //        // destructible/indestructible wall placement
    //        if (id == 1 || id == 2) {
    //            int groupCount = (id == 1) ? (std::rand() % 10 + 10) : (std::rand() % 8 + 8);

    //            for (int g = 0; g < groupCount; ++g) {
    //                int x, y, width, height;

    //                bool placed = false;
    //                for (int attempts = 0; attempts < 10 && !placed; ++attempts) {
    //                    x = std::rand() % m_rows;
    //                    y = std::rand() % m_cols;

    //                    width = 2;
    //                    height = std::rand() % 3 + 2;

    //                    if (CanPlaceGroup(id, x, y, width, height)) {
    //                        GenerateRectangleGroup(id, x, y, width, height);
    //                        placed = true;
    //                    }
    //                }

    //                if (!placed) {
    //                    std::cout << "Warning: Unable to place group for type " << id << " after 10 attempts.\n";
    //                }
    //            }
    //        }
    //    }

    //    // for bomb placing logic
    //    int bombCount = 0;
    //    std::vector<std::pair<int, int>> bombPositions;

    //    for (uint32_t i = 0; i < m_rows; ++i) {
    //        for (uint32_t j = 0; j < m_cols; ++j) {
    //            if (bombCount >= 3) break;

    //            if (m_cellDefinitions.count(3) > 0) {
    //                const auto& bombDefinition = m_cellDefinitions.at(3);

    //                if (bombDefinition.customRule && bombDefinition.customRule(i, j, m_board)) {
    //                    bool validPosition = true;
    //                    for (const auto& [bx, by] : bombPositions) {
    //                        if (SquaredDistance(i, j, bx, by) < 10 * 10) {
    //                            validPosition = false;
    //                            break;
    //                        }
    //                    }
    //                    // for a visualization of bomb placement
    //                    if (validPosition) {
    //                        m_board[i][j] = 3;
    //                        bombPositions.emplace_back(i, j);
    //                        bombCount++;
    //                    }
    //                }
    //            }
    //        }
    //        if (bombCount >= 3) break;
    //    }

    //    std::cout << "Number of bombs placed: " << bombCount << "\n";

    //    for (uint32_t i = 0; i < m_rows; ++i) {
    //        for (uint32_t j = 0; j < m_cols; ++j) {
    //            if (m_board[i][j] == 0) {
    //                m_board[i][j] = 0;
    //            }
    //        }
    //    }
    //}

    void GameBoard::InitializeBoard() {
        ReserveCorners();
        CreateRandomPaths();

        // 1. Plasarea grupurilor de ziduri (indestructibile/destructibile)
        for (const auto& [id, definition] : m_cellDefinitions) {
            if (id == 1 || id == 2) {
                int groupCount = (id == 1) ? (std::rand() % 10 + 10) : (std::rand() % 8 + 8);

                for (int g = 0; g < groupCount; ++g) {
                    bool placed = false;

                    // Generate random groups with a maximum of 10 attempts
                    for (auto _ : std::views::iota(0, 10) | std::views::take_while([&](int) { return !placed; })) {
                        int x = std::rand() % m_rows;
                        int y = std::rand() % m_cols;
                        int width = 2;
                        int height = std::rand() % 3 + 2;

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

        // 3. Resetare celule nefolosite
        for (auto& row : m_board | std::views::all) {
            for (auto& cell : row | std::views::all) {
                if (cell == 0) {
                    cell = 0; // redundant, dar păstrat pentru claritate
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


    void GameBoard::GenerateRectangleGroup(int id, int x, int y, int width, int height) {
        for (int i = 0; i < width; ++i) {
            for (int j = 0; j < height; ++j) {
                int newX = x + i;
                int newY = y + j;

                if (newX >= 0 && newX < m_rows && newY >= 0 && newY < m_cols) {
                    m_board[newX][newY] = id;
                }
            }
        }
    }

    bool GameBoard::CanPlaceGroup(int id, int x, int y, int width, int height) const noexcept {
        for (int i = -1; i <= width; ++i) {
            for (int j = -1; j <= height; ++j) {
                int newX = x + i;
                int newY = y + j;

                if (newX >= 0 && newX < static_cast<int>(m_rows) &&
                    newY >= 0 && newY < static_cast<int>(m_cols)) {
                    if ((m_board[newX][newY] != 0 && m_board[newX][newY] != id) ||
                        ((newX == 0 && newY == 0) ||
                            (newX == 0 && newY == static_cast<int>(m_cols) - 1) ||
                            (newX == static_cast<int>(m_rows) - 1 && newY == 0) ||
                            (newX == static_cast<int>(m_rows) - 1 && newY == static_cast<int>(m_cols) - 1))) {
                        return false;
                    }
                }
            }
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

    bool GameBoard::IsPathPossible(const std::pair<int, int>& start,const  std::pair<int, int>& end) const {
        // use a BFS approach
        std::queue<std::pair<int, int>> toVisit;
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

    void GameBoard::CreateRandomPath(const std::pair<int, int>& start, const std::pair<int, int>& end) {
        if (!IsPathPossible(start, end)) {
            std::cout << "Path not possible, creating a free path.\n";
            ForcePath(start, end);
        }

        int x = start.first;
        int y = start.second;

        m_board[x][y] = 0;

        std::vector<std::pair<int, int>> directions = { {-1, 0}, {1, 0}, {0, -1}, {0, 1} };
        std::random_device rd;
        std::mt19937 gen(rd());

        while (x != end.first || y != end.second) {
            std::shuffle(directions.begin(), directions.end(), gen);
            bool moved = false;

            for (const auto& dir : directions) {
                int newX = x + dir.first;
                int newY = y + dir.second;

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

    void GameBoard::ForcePath(std::pair<int, int> start, std::pair<int, int> end) {
        int x = start.first;
        int y = start.second;

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

std::vector<std::pair<int, int>> MapGen::GameBoard::GetStartingPositions() const noexcept {
    return {
        {0, 0},
        {0, static_cast<int>(m_cols) - 1},
        {static_cast<int>(m_rows) - 1, static_cast<int>(m_cols) - 1},
        {static_cast<int>(m_rows) - 1, 0}
    };
}

