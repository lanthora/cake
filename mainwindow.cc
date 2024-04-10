#include "mainwindow.h"
#include <QApplication>
#include <QGraphicsDropShadowEffect>
#include <QHBoxLayout>
#include <QListWidget>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QPushButton>
#include <QWidgetAction>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("组网工具");
    setFixedSize(850, 610);

    addCentralWidget();
    addFileMenu();
    addEditMenu();
    addHelpMenu();
    addSystemTray();
}

MainWindow::~MainWindow() {}

void MainWindow::closeEvent(QCloseEvent *event)
{
    // 在菜单里选择退出时无需确认直接退出
    if (forceQuit) {
        event->accept();
        return;
    }

    // 点击 X 退出,有系统托盘,发送通知并以托盘形式运行
    if (QSystemTrayIcon ::isSystemTrayAvailable()) {
        if (backgroundNotification) {
            backgroundNotification = false;
            this->trayIcon->showMessage("后台运行", "进程将以托盘的形式在后台持续运行");
        }
        event->ignore();
        hide();
        return;
    }

    // 点击 X 退出,但没有系统托盘,退出前确认
    if (QMessageBox::question(this, "退出", "退出后将断开所有虚拟网络连接,确认退出?")
        == QMessageBox::No) {
        event->ignore();
        return;
    }
    event->accept();
}

void MainWindow::onSystemTrayActivated(QSystemTrayIcon::ActivationReason reason)
{
    // 左键点击托盘图标隐藏或恢复主窗口
    if (reason == QSystemTrayIcon::Trigger) {
        if (isVisible() && !isMinimized()) {
            hide();
        } else {
            showNormal();
        }
    }
}

void MainWindow::quit()
{
    backgroundNotification = false;
    forceQuit = true;
    qApp->quit();
}

void MainWindow::addFileMenu()
{
    QMenu *fileMenu = menuBar()->addMenu("文件");
    QAction *newAction = new QAction("新建");
    QAction *quitAction = new QAction("退出");
    connect(newAction, &QAction::triggered, detailArea, &DetailArea::reset);
    connect(quitAction, &QAction::triggered, this, &MainWindow::quit);

    fileMenu->addAction(newAction);
    fileMenu->addAction(quitAction);
}

void MainWindow::addEditMenu()
{
    QMenu *settingMenu = menuBar()->addMenu("编辑");
    QAction *autoStartAction = new QAction("启动选项");

    settingMenu->addAction(autoStartAction);
}

void MainWindow::addHelpMenu()
{
    QMenu *helpMenu = menuBar()->addMenu("帮助");
    QAction *feedbackAction = new QAction("问题反馈");

    helpMenu->addAction(feedbackAction);
}

void MainWindow::addCentralWidget()
{
    // 右侧保存时追加左侧列表
    detailArea->setCandyList(candyList);

    // 将列表和详细信息区域添加到主窗口
    setCentralWidget(new QWidget);
    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(candyList, 1);
    layout->addWidget(detailArea, 2);
    centralWidget()->setLayout(layout);

    // 连接列表的 itemClicked() 信号到详细信息区域的更新函数
    connect(candyList, &QListWidget::itemClicked, detailArea, &DetailArea::selectItem);
}

void MainWindow::addSystemTray()
{
    // 通过系统托盘退出
    if (QSystemTrayIcon ::isSystemTrayAvailable()) {
        trayIcon = new QSystemTrayIcon(this);
        connect(trayIcon, &QSystemTrayIcon::activated, this, &MainWindow::onSystemTrayActivated);
        trayIcon->setIcon(QIcon(":/logo.png"));
        trayIcon->setToolTip("组网工具");
        trayIcon->show();
    }
}
