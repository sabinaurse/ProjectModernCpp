#include "Map.h"
#include <QPainter>

Map::Map(QGraphicsItem* parent)
    : QGraphicsItem(parent) {}

void Map::setMapData(const std::vector<std::vector<int>>& mapData, std::unordered_map<int, std::string>& cellTypes) {
    m_mapData = mapData;
    m_cellTypes = cellTypes;
    update();
}

QRectF Map::getBoundingRect() const {
    return boundingRect();
}

QRectF Map::boundingRect() const
{
    if (m_mapData.empty()) return QRectF(0, 0, 0, 0);

    return QRectF(0, 0, m_mapData[0].size() * cellSize, m_mapData.size() * cellSize);
}

void Map::updateCell(int x, int y, int newCellType) 
{
    if (x >= 0 && x < m_mapData[0].size() && y >= 0 && y < m_mapData.size()) {
        m_mapData[x][y] = newCellType;
        update();  
    }
}

void Map::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    if (m_mapData.empty()) {
        return;
    }

    for (size_t i = 0; i < m_mapData.size(); ++i) {
        for (size_t j = 0; j < m_mapData[i].size(); ++j)
        {
            QRectF cellRect(j * cellSize, i * cellSize, cellSize, cellSize);

            int cellType = m_mapData[i][j];

            QColor color;
            if (m_cellTypes.count(cellType) > 0)
            {
                const std::string& typeName = m_cellTypes[cellType];

                if (typeName == "Empty") {
                    color = Qt::white;
                }
                else if (typeName == "Destructible Wall") {
                    color = Qt::blue;
                }
                else if (typeName == "Indestructible Wall") {
                    color = Qt::cyan;
                }
                else if (typeName == "Bomb") {
                    color = Qt::red;
                }
                else {
                    color = Qt::black;
                }
            }
            else {
                color = Qt::black;
            }

            painter->setBrush(color);
            painter->drawRect(cellRect);
        }
    }
}

const int Map::cellSize = 30;

