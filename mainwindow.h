#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "candylist.h"
#include "detailarea.h"
#include <QCloseEvent>
#include <QMainWindow>
#include <QSystemTrayIcon>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void onSystemTrayActivated(QSystemTrayIcon::ActivationReason reason);
    void quit();

private:
    void addFileMenu();
    void addEditMenu();
    void addHelpMenu();
    void addCentralWidget();
    void addSystemTray();

private:
    QSystemTrayIcon *trayIcon = nullptr;
    bool backgroundNotification = true;
    bool forceQuit = false;

    CandyList *candyList = nullptr;
    DetailArea *detailArea = nullptr;
};
#endif // MAINWINDOW_H
