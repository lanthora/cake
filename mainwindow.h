#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "candylist.h"
#include "detailarea.h"
#include "switchbutton.h"
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

private:
    void onSystemTrayActivated(QSystemTrayIcon::ActivationReason reason);
    void directQuit();
    QHBoxLayout *createSwitchButtonWidget(QString key, SwitchButton *sb);

private:
    void addFileMenu();
    void addEditMenu();
    void addHelpMenu();
    void addCentralWidget();
    void addSystemTray();
    void startConfig();

private:
    QSystemTrayIcon *trayIcon = nullptr;
    bool backgroundNotification = true;
    bool forceQuit = false;

    CandyList *candyList = new CandyList;
    DetailArea *detailArea = new DetailArea;
};
#endif // MAINWINDOW_H
