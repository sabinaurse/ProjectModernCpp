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

namespace MapGen {

    struct CellTypeDefinition {
        std::string name;
        uint16_t spawnChance;
        std::function<bool(uint32_t, uint32_t, const std::vector<std::vector<int>>&)> customRule;
    };

    class GAMEBOARD_API GameBoard {
    public:
        GameBoard(uint32_t rows, uint32_t cols);
        ~GameBoard();

        void AddCellType(int id, const CellTypeDefinition& definition);
        void InitializeBoard();
        void PrintBoard() const;

    private:
        bool ShouldSpawn(const CellTypeDefinition& definition) const;
        void GenerateRectangleGroup(int id, int x, int y, int width, int height);
        bool CanPlaceGroup(int id, int x, int y, int width, int height) const;
        void ReserveCorners();
        void CreateRandomPaths();
        void CreateRandomPath(std::pair<int, int> start, std::pair<int, int> end);
        bool IsPathPossible(std::pair<int, int> start, std::pair<int, int> end) const;
        void ForcePath(std::pair<int, int> start, std::pair<int, int> end);
    private:
        uint32_t m_rows;
        uint32_t m_cols;
        std::vector<std::vector<int>> m_board;
        std::unordered_map<int, CellTypeDefinition> m_cellDefinitions;
    };
}
