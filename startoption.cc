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
    resize(500, 300);
    setMinimumSize(400, 220);

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
    saveButton->setMaximumWidth(120);
    connect(saveButton, &QPushButton::clicked, this, &StartOption::save);

    QWidget *container = new QWidget(this);
    QVBoxLayout *inner = new QVBoxLayout(container);
    inner->addWidget(autoStartup);
    inner->addWidget(showMainWindow);
    inner->addWidget(updateCheck);
    inner->addWidget(saveButton, 0, Qt::AlignHCenter);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(container, 0, Qt::AlignHCenter);

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
