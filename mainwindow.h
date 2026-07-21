#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "detailarea.h"
#include <QCloseEvent>
#include <QMainWindow>
#include <QSystemTrayIcon>

class TitleBar;
class QMenuBar;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    void onSystemTrayActivated(QSystemTrayIcon::ActivationReason reason);
    void directQuit();
    bool confirmQuit();

private:
    void addFileMenu();
    void addEditMenu();
    void addHelpMenu();
    void addSystemTray();

private:
    TitleBar *titleBar = nullptr;
    QMenuBar *mainMenuBar = nullptr;
    QSystemTrayIcon *trayIcon = nullptr;
    bool forceQuit = false;

    DetailArea *detailArea = new DetailArea;
    QSettings settings;
};
#endif // MAINWINDOW_H
