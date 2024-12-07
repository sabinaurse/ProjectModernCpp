#include "ClientInterface.h"
#include "LoginPage.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    LoginPage loginPage;
    loginPage.show();
    return a.exec();
}
