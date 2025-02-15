﻿#include "LoginPage.h"

LoginPage::LoginPage(ClientRequests* requests, QWidget* parent)
    : QWidget(parent), m_clientRequests(requests),
    m_nameRegex(QRegularExpression("^[a-zA-Z0-9]{5,18}$"))
{
    ui.setupUi(this);

    ui.loginInput->setFixedSize(160, 30);
    ui.registerButton->setFixedSize(160, 30);  
    ui.loginButton->setFixedSize(160, 30);

    QVBoxLayout* layout = new QVBoxLayout();
    layout->addStretch();
    layout->addWidget(ui.loginInput);
    layout->addWidget(ui.loginButton);
    layout->addWidget(ui.registerButton);
    layout->addStretch();

    QHBoxLayout* centralLayout = new QHBoxLayout(this);
    centralLayout->addStretch();  
    centralLayout->addLayout(layout);  
    centralLayout->addStretch();  


    connect(ui.loginButton, &QPushButton::clicked, this, &LoginPage::onLoginClicked);
    connect(ui.registerButton, &QPushButton::clicked, this, &LoginPage::onRegisterClicked);

    connect(m_clientRequests, &ClientRequests::loginCompleted, this, &LoginPage::onRequestCompleted);
    connect(m_clientRequests, &ClientRequests::requestFailed, this, &LoginPage::onRequestFailed);
}

LoginPage::~LoginPage() {
    delete m_clientRequests;  
}

void LoginPage::onLoginClicked() 
{
    QString playerName = ui.loginInput->text(); 
   
    if (playerName.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please enter a player name.");
        return;
    }

    if (!m_nameRegex.match(playerName).hasMatch()) {
        QMessageBox::warning(this, "Invalid Name",
            "Name must be 5-18 characters long, contain only letters and digits, and have no spaces or special characters.");
        return;
    }

    m_clientRequests->GetPlayer(playerName);
}

void LoginPage::onRegisterClicked() 
{
    QString playerName = ui.loginInput->text();  

    if (playerName.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please enter a player name.");
        return;
    }

    if (!m_nameRegex.match(playerName).hasMatch()) {
        QMessageBox::warning(this, "Invalid Name",
            "Name must be 5-18 characters long, contain only letters and digits, and have no spaces or special characters.");
        return;
    }

    m_clientRequests->CreatePlayer(playerName);  
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
            int cooldown = jsonObj["cooldown_level"].toInt();
            int bulletSpeed = jsonObj["bullet_speed_level"].toInt();

            ClientState::instance().SetCurrentPlayer(playerName);
            ClientState::instance().SetPlayerScore(playerScore);
            ClientState::instance().SetPlayerPoints(playerPoints);
            ClientState::instance().SetCooldownLevel(cooldown);
            ClientState::instance().SetBulletSpeedLevel(bulletSpeed);

            emit loginSuccessful(playerName, playerScore, playerPoints, cooldown, bulletSpeed);
        }
    }
}


void LoginPage::onRequestFailed(const QString& error) 
{
    QMessageBox::warning(this, "Error", "Request failed: " + error);
}
