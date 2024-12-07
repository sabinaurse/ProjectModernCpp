#include "LoginPage.h"
#include <QMessageBox>

LoginPage::LoginPage(QWidget* parent)
    : QWidget(parent), clientRequests(new ClientRequests(this)) 
{
    ui.setupUi(this);  

    connect(ui.loginButton, &QPushButton::clicked, this, &LoginPage::onLoginClicked);
    connect(ui.registerButton, &QPushButton::clicked, this, &LoginPage::onRegisterClicked);

    connect(clientRequests, &ClientRequests::requestCompleted, this, &LoginPage::onRequestCompleted);
    connect(clientRequests, &ClientRequests::requestFailed, this, &LoginPage::onRequestFailed);
}

LoginPage::~LoginPage() {
    delete clientRequests;  
}

void LoginPage::onLoginClicked() 
{
    QString playerName = ui.loginInput->text(); 
   
    if (playerName.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please enter a player name.");
        return;
    }

    clientRequests->GetPlayer(playerName);
}

void LoginPage::onRegisterClicked() 
{
    QString playerName = ui.loginInput->text();  

    if (playerName.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please enter a player name.");
        return;
    }

    clientRequests->CreatePlayer(playerName);  
}

void LoginPage::onRequestCompleted(const QString& data) {

    if (data.contains("Player added", Qt::CaseInsensitive)) {
        QMessageBox::information(this, "Success", "Player created successfully!");
    }
    else {
        QJsonDocument jsonDoc = QJsonDocument::fromJson(data.toUtf8());
        if (jsonDoc.isObject()) {
            QJsonObject jsonObj = jsonDoc.object();

            QString playerName = jsonObj["name"].toString();
            int playerScore = jsonObj["score"].toInt();
            int playerPoints = jsonObj["points"].toInt();

            ClientState::instance().SetCurrentPlayer(playerName);
            ClientState::instance().SetPlayerScore(playerScore);
            ClientState::instance().SetPlayerPoints(playerPoints);

            MainPage* mainPage = new MainPage();
            mainPage->displayPlayerInfo(QString("Name: %1\nScore: %2\nPoints: %3")
                .arg(playerName)
                .arg(playerScore)
                .arg(playerPoints));  

            mainPage->show();
            close();
        }
    }
}


void LoginPage::onRequestFailed(const QString& error) 
{
    QMessageBox::warning(this, "Error", "Request failed: " + error);
}
