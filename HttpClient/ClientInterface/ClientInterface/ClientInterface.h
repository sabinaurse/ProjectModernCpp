#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_ClientInterface.h"

class ClientInterface : public QMainWindow
{
    Q_OBJECT

public:
    ClientInterface(QWidget *parent = nullptr);
    ~ClientInterface();

private:
    Ui::ClientInterfaceClass ui;
};
