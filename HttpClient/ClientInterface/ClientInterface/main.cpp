#include <QDir>
#include <QCoreApplication>
#include "ClientInterface.h"
#include "LoginPage.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ClientInterface clientInterface;
    clientInterface.show();
    return a.exec();
}
