#include "mainwindow.h"
#include <QApplication>
#include <QDir>
#include <QMessageBox>
#include <QSharedMemory>
#include <spdlog/spdlog.h>

#ifdef Q_OS_WIN
#include <spdlog/sinks/rotating_file_sink.h>
#endif

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
        QMessageBox::warning(nullptr, "Cake", "Another instance is already running");
        return 0;
    }
    shared.create(1);

    spdlog::set_level(spdlog::level::debug);

#ifdef Q_OS_WIN
    QDir().mkpath("C:/ProgramData/Cake/logs");
    auto max_size = 1048576 * 5;
    auto max_files = 3;
    auto logger = spdlog::rotating_logger_mt("candy", "C:/ProgramData/Cake/logs/candy.txt", max_size, max_files, true);
    spdlog::set_default_logger(logger);
    spdlog::flush_every(std::chrono::seconds(1));
#endif

    MainWindow w;
    return a.exec();
}
