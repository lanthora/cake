#include "confirmdialog.h"
#include "mainwindow.h"
#include "privilege.h"
#include <QApplication>
#include <QDir>
#include <QFile>
#include <QPalette>
#include <QProcess>
#include <QSharedMemory>
#include <QStandardPaths>
#include <QStyleFactory>
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
    QStyle *fusionStyle = QStyleFactory::create("Fusion");
    QApplication::setStyle(fusionStyle);
    QPalette palette = fusionStyle->standardPalette();
    palette.setColor(QPalette::Window, QColor("#f5f6f8"));
    palette.setColor(QPalette::AlternateBase, QColor("#f5f6f8"));
    a.setPalette(palette);

    QFile styleFile(":/style.qss");
    if (styleFile.open(QFile::ReadOnly)) {
        a.setStyleSheet(QString::fromUtf8(styleFile.readAll()));
        styleFile.close();
    }

#ifdef Q_OS_LINUX
    while (!privilege::hasNetAdmin()) {
        ConfirmDialog dialog("Cake", "Cake needs the CAP_NET_ADMIN privilege to create the virtual network device.\n\nGrant it now?");
        dialog.setConfirmButton("Authorize", ConfirmDialog::PrimaryButton);
        if (dialog.exec() != QDialog::Accepted)
            return 0;

        if (privilege::fixWithPkexec(QCoreApplication::applicationFilePath())) {
            QProcess::startDetached(QCoreApplication::applicationFilePath(),
                                    QApplication::arguments().mid(1));
            return 0;
        }

        ConfirmDialog failed("Cake", "Failed to grant CAP_NET_ADMIN. Please try again.");
        failed.setConfirmButton("OK", ConfirmDialog::PrimaryButton);
        failed.exec();
    }
#endif

    QSharedMemory shared("canets.org/cake");
    if (shared.attach()) {
        ConfirmDialog msgBox("Cake", "Another instance is already running");
        msgBox.setConfirmButton("OK", ConfirmDialog::PrimaryButton);
        msgBox.exec();
        return 0;
    }
    shared.create(1);

    PocoLogGuard logGuard;
    MainWindow w;
    return a.exec();
}
