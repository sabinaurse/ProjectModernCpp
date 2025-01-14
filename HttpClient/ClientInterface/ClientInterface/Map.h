#pragma once
#include <QGraphicsItem>
#include <vector>
#include <unordered_map>

class Map : public QGraphicsItem {
private:
    std::vector<std::vector<int>> m_mapData;
    std::unordered_map<int, std::string> m_cellTypes;
    static const int cellSize;

protected:
    QRectF boundingRect() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) override;

public:
    explicit Map(QGraphicsItem* parent = nullptr);
    static int getCellSize() { return cellSize; }
    void updateCell(int x, int y, int newCellType);
    QRectF getBoundingRect() const;
    void setMapData(const std::vector<std::vector<int>>& mapData, std::unordered_map<int, std::string>& cellTypes);

};


