#include "mainwindow.h"
#include <QApplication>
#include <QDir>
#include <QMessageBox>
#include <QSharedMemory>
#include <QStandardPaths>
#include <Poco/AutoPtr.h>
#include <Poco/FileChannel.h>
#include <Poco/FormattingChannel.h>
#include <Poco/Logger.h>
#include <Poco/PatternFormatter.h>

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

class PocoLogGuard
{
public:
    PocoLogGuard()
    {
        QString logDir;
#ifdef Q_OS_WIN
        logDir = "C:/ProgramData/Cake/logs";
#else
        logDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/logs";
#endif
        QDir().mkpath(logDir);

        Poco::AutoPtr<Poco::FileChannel> fileChannel(new Poco::FileChannel);
        fileChannel->setProperty("path", (logDir + "/candy.txt").toStdString());
        fileChannel->setProperty("rotation", "5 M");
        fileChannel->setProperty("archive", "number");
        fileChannel->setProperty("purgeCount", "3");
        fileChannel->setProperty("flush", "true");

        Poco::AutoPtr<Poco::PatternFormatter> formatter(new Poco::PatternFormatter);
        formatter->setProperty("pattern", "%Y-%m-%d %H:%M:%S [%q] %t");

        Poco::AutoPtr<Poco::FormattingChannel> channel(new Poco::FormattingChannel(formatter, fileChannel));
        Poco::Logger::root().setChannel(channel);
        Poco::Logger::root().setLevel("debug");
    }

    ~PocoLogGuard()
    {
        Poco::Logger::root().setChannel(nullptr);
    }

    PocoLogGuard(const PocoLogGuard &) = delete;
    PocoLogGuard &operator=(const PocoLogGuard &) = delete;
};

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
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::NoIcon);
        msgBox.setWindowTitle("Cake");
        msgBox.setText("Another instance is already running");
        msgBox.exec();
        return 0;
    }
    shared.create(1);

    PocoLogGuard logGuard;
    MainWindow w;
    return a.exec();
}
