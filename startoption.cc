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
    setWindowTitle("Startup");
    resize(400, 240);
    setMinimumSize(360, 200);

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

    updateCheck->setChecked(settings.value("updatecheck", true).toBool());

    QPushButton *saveButton = new QPushButton("Save", this);
    saveButton->setObjectName("primaryButton");
    saveButton->setMaximumWidth(100);
    connect(saveButton, &QPushButton::clicked, this, &StartOption::save);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(24, 20, 24, 20);
    layout->setSpacing(10);
    layout->addWidget(autoStartup);
    layout->addWidget(showMainWindow);
    layout->addWidget(updateCheck);
    layout->addSpacing(8);
    layout->addWidget(saveButton, 0, Qt::AlignHCenter);

    save();
}

void StartOption::showEvent(QShowEvent *event)
{
    if (QSysInfo::productType() == "windows") {
        autoStartup->setChecked(settings.value("autostartup", true).toBool());
    }

    if (QSystemTrayIcon::isSystemTrayAvailable()) {
        showMainWindow->setChecked(settings.value("showmainwindow", true).toBool());
    }

    QWidget::showEvent(event);
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

    settings.setValue("updatecheck", updateCheck->isChecked());

    close();
}
