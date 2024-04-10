#include "mainwindow.h"
#include <QApplication>
#include <QMessageBox>
#include <QSharedMemory>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QSharedMemory shared("lanthora/cake");
    if (shared.attach()) {
        QMessageBox::warning(nullptr, "组网工具", "已经有一个实例正在运行");
        return 0;
    }
    shared.create(1);

    MainWindow w;
    w.show();
    return a.exec();
}
