#include "candy.h"
#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    void *candy = candy_client_create();
    candy_client_release(candy);

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
