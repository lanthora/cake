#include "mainwindow.h"
#include "about.h"
#include "confirmdialog.h"
#include "feedback.h"
#include "startoption.h"
#include "titlebar.h"
#include "update.h"
#include <QApplication>
#include <QDesktopServices>
#include <QFile>
#include <QGraphicsDropShadowEffect>
#include <QHBoxLayout>
#include <QIcon>
#include <QMenu>
#include <QMenuBar>
#include <QPushButton>
#include <QStatusBar>
#include <QVBoxLayout>
#include <QWidgetAction>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Cake");
    setWindowFlags(windowFlags() | Qt::FramelessWindowHint);
    resize(900, 640);
    setMinimumSize(760, 560);
    setWindowIcon(QIcon(":/logo.ico"));

    QFile styleFile(":/style.qss");
    if (styleFile.open(QFile::ReadOnly)) {
        setStyleSheet(styleFile.readAll());
        styleFile.close();
    }

    titleBar = new TitleBar("Cake", this);
    mainMenuBar = new QMenuBar(this);
    mainMenuBar->setNativeMenuBar(false);

    QWidget *central = new QWidget(this);
    auto *centralLayout = new QVBoxLayout(central);
    centralLayout->setContentsMargins(0, 0, 0, 0);
    centralLayout->setSpacing(0);
    centralLayout->addWidget(titleBar);
    centralLayout->addWidget(mainMenuBar);
    detailArea->setParent(central);
    centralLayout->addWidget(detailArea);
    setCentralWidget(central);

    addFileMenu();
    addEditMenu();
    addHelpMenu();
    addSystemTray();

    if (settings.value("showmainwindow", true).toBool()) {
        show();
    }

    Update *update = new Update(this);
    connect(update, &Update::notify, [&](QString current, QString latest) {
        show();
        ConfirmDialog dialog("Update", "New version available: " + latest, this);
        dialog.setConfirmButton("Download", ConfirmDialog::PrimaryButton);
        dialog.setCancelButton("Later");
        if (dialog.exec() == QDialog::Accepted) {
            QDesktopServices::openUrl(QUrl("https://github.com/lanthora/cake/releases/latest"));
        }
    });
}

MainWindow::~MainWindow() {}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (forceQuit) {
        event->accept();
        return;
    }

    if (QSystemTrayIcon::isSystemTrayAvailable()) {
        if (settings.value("backgroundnotification", true).toBool()) {
            settings.setValue("backgroundnotification", false);
            settings.sync();
            this->trayIcon->showMessage("Background", "Running in system tray");
        }
        event->ignore();
        hide();
        return;
    }

    if (!confirmQuit()) {
        event->ignore();
        return;
    }
    event->accept();
}

void MainWindow::onSystemTrayActivated(QSystemTrayIcon::ActivationReason reason)
{
    if (reason == QSystemTrayIcon::Trigger) {
        showNormal();
        activateWindow();
    }
}

void MainWindow::directQuit()
{
    forceQuit = true;
    qApp->quit();
}

bool MainWindow::confirmQuit()
{
    ConfirmDialog dialog("Quit", "Quitting will disconnect all networks. Continue?", this);
    dialog.setConfirmButton("Quit", ConfirmDialog::DangerButton);
    dialog.setCancelButton("Cancel");
    return dialog.exec() == QDialog::Accepted;
}

void MainWindow::addFileMenu()
{
    QMenu *fileMenu = mainMenuBar->addMenu("File");
    QAction *newAction = new QAction("New", fileMenu);
    QAction *quitAction = new QAction("Quit", fileMenu);
    connect(newAction, &QAction::triggered, [this] { detailArea->reset(true); });
    connect(quitAction, &QAction::triggered, [this] {
        if (confirmQuit()) {
            directQuit();
        }
    });

    fileMenu->addAction(newAction);
    fileMenu->addAction(quitAction);
}

void MainWindow::addEditMenu()
{
    QMenu *settingMenu = mainMenuBar->addMenu("Settings");
    QAction *autoStartAction = new QAction("Startup", settingMenu);

    StartOption *startOption = new StartOption(this);
    connect(autoStartAction, &QAction::triggered, startOption, &StartOption::show);

    settingMenu->addAction(autoStartAction);
}

void MainWindow::addHelpMenu()
{
    QMenu *helpMenu = mainMenuBar->addMenu("Help");

    QAction *feedbackAction = new QAction("Feedback", helpMenu);
    Feedback *feedback = new Feedback(this);

    QAction *aboutAction = new QAction("About", helpMenu);
    About *about = new About(this);

    connect(feedbackAction, &QAction::triggered, feedback, &Feedback::show);
    connect(aboutAction, &QAction::triggered, about, &About::show);

    helpMenu->addAction(feedbackAction);
    helpMenu->addAction(aboutAction);
}

void MainWindow::addSystemTray()
{
    if (QSystemTrayIcon::isSystemTrayAvailable()) {
        QMenu *trayMenu = new QMenu(this);
        QAction *quitAction = new QAction("Quit", trayMenu);
        connect(quitAction, &QAction::triggered, this, &MainWindow::directQuit);

        trayMenu->addAction(quitAction);

        trayIcon = new QSystemTrayIcon(this);
        connect(trayIcon, &QSystemTrayIcon::activated, this, &MainWindow::onSystemTrayActivated);
        trayIcon->setIcon(QIcon(":/logo.ico"));
        trayIcon->setToolTip("Cake");
        trayIcon->setContextMenu(trayMenu);
        trayIcon->show();
    }
}
