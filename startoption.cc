#include "startoption.h"
#include <QApplication>
#include <QDir>
#include <QLabel>
#include <QPushButton>
#include <QSysInfo>
#include <QSystemTrayIcon>
#include <QVBoxLayout>

static QWidget *wrapOption(QCheckBox *checkBox, const QString &hint)
{
    QWidget *wrapper = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout(wrapper);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(2);
    layout->addWidget(checkBox);
    QLabel *hintLabel = new QLabel(hint, wrapper);
    hintLabel->setObjectName("dialogHint");
    hintLabel->setWordWrap(true);
    hintLabel->setContentsMargins(26, 0, 0, 0);
    layout->addWidget(hintLabel);
    return wrapper;
}

StartOption::StartOption(QWidget *parent)
    : FramelessDialog("Startup", parent)
{
    resize(440, 340);
    setMinimumSize(400, 300);

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

    QLabel *subtitle = new QLabel("Configure how Cake behaves on startup.", this);
    subtitle->setObjectName("dialogSubtitle");
    subtitle->setWordWrap(true);

    QPushButton *saveButton = new QPushButton("Save", this);
    saveButton->setObjectName("primaryButton");
    saveButton->setMaximumWidth(100);
    connect(saveButton, &QPushButton::clicked, this, &StartOption::save);

    QVBoxLayout *layout = contentLayout();
    layout->setContentsMargins(32, 20, 32, 20);
    layout->setSpacing(8);
    layout->addWidget(subtitle);
    layout->addSpacing(8);
    layout->addWidget(wrapOption(autoStartup, "Automatically launch Cake when you sign in to your system."));
    layout->addSpacing(4);
    layout->addWidget(wrapOption(showMainWindow, "Display the main window when Cake starts."));
    layout->addSpacing(4);
    layout->addWidget(wrapOption(updateCheck, "Automatically check for new versions on startup."));
    layout->addStretch();
    layout->addWidget(saveButton, 0, Qt::AlignRight);

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
