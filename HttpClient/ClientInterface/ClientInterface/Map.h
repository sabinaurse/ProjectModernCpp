#pragma once
#include <QGraphicsItem>
#include <QPainter>
#include <vector>
#include <unordered_map>

class Map : public QGraphicsItem {
public:
    explicit Map(QGraphicsItem* parent = nullptr);
    static int getCellSize() { return cellSize; }
    QRectF getBoundingRect() const;
    void setMapData(const std::vector<std::vector<int>>& mapData);
    void setMapData(const std::vector<std::vector<int>>& mapData, std::unordered_map<int, std::string>& cellTypes);

protected:
    QRectF boundingRect() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) override;

private:
    std::vector<std::vector<int>> m_mapData;
    std::unordered_map<int, std::string> m_cellTypes;
    static const int cellSize;
};


