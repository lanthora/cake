#include "mainwindow.h"
#include <QApplication>
#include <QMessageBox>
#include <QSharedMemory>

int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName("canets");
    QCoreApplication::setOrganizationDomain("canets.org");
    QCoreApplication::setApplicationName("cake");

    QApplication a(argc, argv);
    QSharedMemory shared("canets.org/cake");
    if (shared.attach()) {
        QMessageBox::warning(nullptr, "组网工具", "已经有一个实例正在运行");
        return 0;
    }
    shared.create(1);

    MainWindow w;
    return a.exec();
}
