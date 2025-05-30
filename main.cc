#include "mainwindow.h"
#include <QApplication>
#include <QDir>
#include <QMessageBox>
#include <QSharedMemory>

#ifdef Q_OS_WIN
void configureWerDumpSettings()
{
    const QString dumpPath = "C:/ProgramData/Cake/dmps/";
    QDir().mkpath(dumpPath);
    QString regPath = "HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\Windows Error Reporting\\LocalDumps\\cake.exe";
    QSettings werSettings(regPath, QSettings::NativeFormat);
    werSettings.setValue("DumpFolder", dumpPath);
    werSettings.setValue("DumpCount", 3);
    werSettings.setValue("DumpType", 2);
    werSettings.sync();
}
#endif

int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName("canets");
    QCoreApplication::setOrganizationDomain("canets.org");
    QCoreApplication::setApplicationName("cake");

#ifdef Q_OS_WIN
    configureWerDumpSettings();
#endif

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
