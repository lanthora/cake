#include "startoption.h"
#include <QApplication>
#include <QDir>
#include <QPushButton>
#include <QSysInfo>
#include <QSystemTrayIcon>
#include <QVBoxLayout>

StartOption::StartOption(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("启动选项");
    setFixedSize(300, 200);

    QPushButton *saveButton = new QPushButton("保存", this);
    connect(saveButton, &QPushButton::clicked, this, &StartOption::save);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(autoStartup, 0, Qt::AlignCenter);
    layout->addWidget(showMainWindow, 0, Qt::AlignCenter);
    layout->addWidget(saveButton, 0, Qt::AlignCenter);

    save();
}

void StartOption::showEvent(QShowEvent *event)
{
    if (QSysInfo::productType() == "windows") {
        autoStartup->setChecked(settings.value("autostartup", true).toBool());
    } else {
        autoStartup->setChecked(false);
        autoStartup->setDisabled(true);
    }

    if (QSystemTrayIcon::isSystemTrayAvailable()) {
        showMainWindow->setChecked(settings.value("showmainwindow", true).toBool());
    } else {
        showMainWindow->setChecked(true);
        showMainWindow->setDisabled(true);
    }
}

void StartOption::save()
{
    if (QSysInfo::productType() == "windows") {
        settings.setValue("autostartup", autoStartup->isChecked());

        QSettings reg("HKLM\\SOFTWARE\\WOW6432Node\\Microsoft\\Windows\\CurrentVersion\\Run", QSettings::NativeFormat);
        QString filePath = QCoreApplication::applicationFilePath();
        QFileInfo fileInfo(filePath);
        QString fileName = fileInfo.fileName();
        if (autoStartup->isChecked()) {
            reg.setValue(fileName, QDir::toNativeSeparators(filePath));
        } else {
            reg.remove(fileName);
        }
        reg.sync();
    }

    if (QSystemTrayIcon::isSystemTrayAvailable()) {
        settings.setValue("showmainwindow", showMainWindow->isChecked());
    }

    hide();
}
