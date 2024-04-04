#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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
    QSystemTrayIcon *trayIcon = nullptr;
    bool backgroundNotification = true;
};
#endif // MAINWINDOW_H
