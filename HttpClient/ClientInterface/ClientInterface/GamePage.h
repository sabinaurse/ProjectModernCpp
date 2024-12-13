#ifndef GAMEPAGE_H
#define GAMEPAGE_H

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QObject>
#include <QTimer>
#include <ClientRequests.h>
#include <ClientState.h>
#include <Penguin.h>

class GamePage : public QWidget {
	Q_OBJECT
public:
	explicit GamePage(ClientRequests* requests, QWidget* parent);
	~GamePage();
private slots:
	void onGameStateUpdated();
private:
	QGraphicsScene* m_scene;   
	QGraphicsView* m_view;     
	Penguin* m_penguin;
	ClientRequests* m_requests;
};

#endif

