#include "mainwindow.h"
#include "candyitem.h"
#include "candylist.h"
#include "detailarea.h"
#include <QApplication>
#include <QHBoxLayout>
#include <QListWidget>
#include <QMenu>
#include <QMessageBox>
#include <QWidgetAction>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // 设置窗口标题
    setWindowTitle("Cake");

    // 创建左侧列表
    CandyList *list = new CandyList;
    list->addItem(new CandyItem);
    list->addItem(new CandyItem);

    // 创建右侧详细信息区域
    DetailArea *detailArea = new DetailArea;

    // 将列表和详细信息区域添加到主窗口
    setCentralWidget(new QWidget);
    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(list, 1);
    layout->addWidget(detailArea, 3);
    centralWidget()->setLayout(layout);

    // 连接列表的 itemClicked() 信号到详细信息区域的更新函数
    connect(list, &QListWidget::itemClicked, detailArea, &DetailArea::update);

    // 通过系统托盘退出
    if (QSystemTrayIcon ::isSystemTrayAvailable()) {
        // TODO: 替换成好看的图标
        QIcon quitIcon = QApplication::style()->standardIcon(QStyle::SP_TabCloseButton);
        QIcon logoIcon = QApplication::style()->standardIcon(QStyle::SP_TitleBarMenuButton);

        QAction *quitAction = new QAction(quitIcon, "Quit");
        connect(quitAction, &QAction::triggered, this, &MainWindow::quit);

        QMenu *trayMenu = new QMenu(this);
        trayMenu->addAction(quitAction);

        trayIcon = new QSystemTrayIcon(this);
        connect(trayIcon, &QSystemTrayIcon::activated, this, &MainWindow::onSystemTrayActivated);
        trayIcon->setIcon(logoIcon);
        trayIcon->setToolTip("Candy");
        trayIcon->setContextMenu(trayMenu);
        trayIcon->show();
    }
}

MainWindow::~MainWindow() {}

void MainWindow::closeEvent(QCloseEvent *event)
{
    // 有系统托盘,后台运行
    if (QSystemTrayIcon ::isSystemTrayAvailable()) {
        if (backgroundNotification) {
            backgroundNotification = false;
            this->trayIcon->showMessage("后台运行", "进程将以托盘的形式在后台持续运行");
        }
        event->ignore();
        hide();
        return;
    }

    // 没有系统托盘,退出前确认
    if (QMessageBox::question(this, "退出", "退出后将断开所有虚拟网络连接,确认退出?")
        == QMessageBox::No) {
        event->ignore();
        return;
    }

    // 退出
    event->accept();
}

void MainWindow::onSystemTrayActivated(QSystemTrayIcon::ActivationReason reason)
{
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
    qApp->quit();
}
