#ifdef GAMEBOARD_DLL_EXPORTS
#define GAMEBOARD_API __declspec(dllexport)
#else
#define GAMEBOARD_API __declspec(dllimport)
#endif

#pragma once
#include <vector>
#include <unordered_map>
#include <string>
#include <functional>
#include <random>

namespace MapGen {

    struct CellTypeDefinition {
        std::string name;
        uint8_t spawnChance;
        std::function<bool(uint32_t, uint32_t, const std::vector<std::vector<uint32_t>>&)> customRule;
    };

    class GAMEBOARD_API GameBoard {
    public:
        GameBoard(uint32_t rows, uint32_t cols);
        GameBoard(GameBoard&& other) noexcept;
        GameBoard& operator=(GameBoard&& other) noexcept;
        ~GameBoard();

        uint32_t GetRows() const;
        uint32_t GetCols() const;
        std::vector<std::vector<uint32_t>>& GetBoard();
        const std::vector<std::vector<uint32_t>>& GetBoard() const;
        std::vector<std::pair<uint32_t, uint32_t>> GetStartingPositions() const noexcept;

        void AddCellType(uint8_t id, CellTypeDefinition definition);
        void InitializeBoard();
        void PrintBoard() const;

    private:
        bool ShouldSpawn(const CellTypeDefinition& definition) const;
        void GenerateRectangleGroup(uint8_t id, uint32_t x, uint32_t y, uint32_t width, uint32_t height);
        bool CanPlaceGroup(uint8_t id, uint32_t x, uint32_t y, uint32_t width, uint32_t height) const noexcept;
        void ReserveCorners();

        void CreateRandomPaths();
        void CreateRandomPath(const std::pair<uint32_t, uint32_t>& start,const std::pair<uint32_t, uint32_t>& end);
        bool IsPathPossible(const std::pair<uint32_t, uint32_t>& start, const std::pair<uint32_t, uint32_t>& end) const;
        void ForcePath(std::pair<uint32_t, uint32_t> start, std::pair<uint32_t, uint32_t> end);
    private:
        uint32_t m_rows = 0;
        uint32_t m_cols = 0;
        std::vector<std::vector<uint32_t>> m_board;
        std::vector<std::tuple<uint32_t, uint32_t, uint32_t, uint32_t>> m_placedGroups;
        std::unordered_map<uint8_t, CellTypeDefinition> m_cellDefinitions;
        mutable std::mt19937 m_rng;
    };
}
