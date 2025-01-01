#pragma once

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QObject>
#include <QTimer>
#include <QVBoxLayout>

#include "ClientRequests.h"
#include "ClientState.h"
#include "Penguin.h"
#include "Map.h"

class GamePage : public QWidget {
	Q_OBJECT

private:
	QGraphicsScene* m_scene;
	QGraphicsView* m_view;
	Penguin* m_penguin;
	ClientRequests* m_requests;
	Map* m_map;

private slots:
	void onGameStateUpdated();
	void onMapReceived(const std::vector<std::vector<int>>& mapData, std::unordered_map<int, std::string>& cellTypes);

public:
	explicit GamePage(ClientRequests* requests, QWidget* parent);
	~GamePage();
};



