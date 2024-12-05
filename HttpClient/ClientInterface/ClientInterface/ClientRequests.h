#ifndef CLIENTREQUESTS_H
#define CLIENTREQUESTS_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonObject>

class ClientRequests : public QObject {
    Q_OBJECT

public:
    explicit ClientRequests(QObject* parent = nullptr);

    void CreatePlayer(const QString& name);
    void GetPlayer(const QString& name);
    void DeletePlayer(const QString& name);
    void UpdatePlayer(const QString& name, int newScore, int newPoints);
    void GetMap();
    void StartGame();
    void ResetGame();
    void GetLeaderboard();
signals:
    void requestCompleted(const QString& response);
    void requestFailed(const QString& error);

private slots:
    void onReplyFinished(QNetworkReply* reply);

private:
    QNetworkAccessManager* networkManager;
};

#endif 
