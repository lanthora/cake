#include "mainwindow.h"
#include "about.h"
#include "feedback.h"
#include "startoption.h"
#include "update.h"
#include <QApplication>
#include <QDesktopServices>
#include <QGraphicsDropShadowEffect>
#include <QHBoxLayout>
#include <QIcon>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QPushButton>
#include <QStatusBar>
#include <QWidgetAction>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Cake");
    resize(600, 590);
    setMinimumSize(600, 400);
    setWindowIcon(QIcon(":/logo.ico"));

    addCentralWidget();
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
        if (QMessageBox::question(this, "Update", QString("New version available: " + latest)) == QMessageBox::Yes) {
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

    if (QMessageBox::question(this, "Quit", "Quitting will disconnect all networks. Continue?") == QMessageBox::No) {
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

void MainWindow::addFileMenu()
{
    QMenu *fileMenu = menuBar()->addMenu("File");
    QAction *newAction = new QAction("New", fileMenu);
    QAction *quitAction = new QAction("Quit", fileMenu);
    connect(newAction, &QAction::triggered, [this] { detailArea->reset(true); });
    connect(quitAction, &QAction::triggered, [this] {
        if (QMessageBox::question(this, "Quit", "Quitting will disconnect all networks. Continue?") == QMessageBox::Yes) {
            directQuit();
        }
    });

    fileMenu->addAction(newAction);
    fileMenu->addAction(quitAction);
}

void MainWindow::addEditMenu()
{
    QMenu *settingMenu = menuBar()->addMenu("Settings");
    QAction *autoStartAction = new QAction("Startup", settingMenu);

    StartOption *startOption = new StartOption(this);
    connect(autoStartAction, &QAction::triggered, startOption, &StartOption::show);

    settingMenu->addAction(autoStartAction);
}

void MainWindow::addHelpMenu()
{
    QMenu *helpMenu = menuBar()->addMenu("Help");

    QAction *feedbackAction = new QAction("Feedback", helpMenu);
    Feedback *feedback = new Feedback(this);

    QAction *aboutAction = new QAction("About", helpMenu);
    About *about = new About(this);

    connect(feedbackAction, &QAction::triggered, feedback, &Feedback::show);
    connect(aboutAction, &QAction::triggered, about, &About::show);

    helpMenu->addAction(feedbackAction);
    helpMenu->addAction(aboutAction);
}

void MainWindow::addCentralWidget()
{
    detailArea->setParent(this);
    setCentralWidget(detailArea);

    connect(detailArea, &DetailArea::updateTitle, this, &MainWindow::setWindowTitle);
}

void MainWindow::addSystemTray()
{
    if (QSystemTrayIcon::isSystemTrayAvailable()) {
        QMenu *trayMenu = new QMenu(this);
        QIcon quitIcon = QApplication::style()->standardIcon(QStyle::SP_TabCloseButton);
        QAction *quitAction = new QAction(quitIcon, "Quit", trayMenu);
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
