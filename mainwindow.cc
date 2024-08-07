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
#include <QListWidget>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QPushButton>
#include <QStatusBar>
#include <QWidgetAction>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("组网工具");
    setFixedSize(850, 570);
    setWindowIcon(QIcon(":/logo.ico"));

    addCentralWidget();
    addFileMenu();
    addEditMenu();
    addHelpMenu();
    addSystemTray();

    // 显示主界面
    if (settings.value("showmainwindow", true).toBool()) {
        show();
    }

    Update *update = new Update(this);
    connect(update, &Update::notify, [&](QString current, QString latest) {
        show();
        if (QMessageBox::question(this, "更新提示", QString("检查到新版本: " + latest + ", 请及时更新")) == QMessageBox::Yes) {
            QDesktopServices::openUrl(QUrl("https://github.com/lanthora/cake/releases/latest"));
        }
    });
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
    if (QSystemTrayIcon::isSystemTrayAvailable()) {
        if (settings.value("backgroundnotification", true).toBool()) {
            settings.setValue("backgroundnotification", false);
            settings.sync();
            this->trayIcon->showMessage("后台运行", "进程将以托盘的形式在后台持续运行");
        }
        event->ignore();
        hide();
        return;
    }

    // 点击 X 退出,但没有系统托盘,退出前确认
    if (QMessageBox::question(this, "退出", "退出后将断开所有虚拟网络连接,确认退出?") == QMessageBox::No) {
        event->ignore();
        return;
    }
    event->accept();
}

void MainWindow::onSystemTrayActivated(QSystemTrayIcon::ActivationReason reason)
{
    // 左键点击托盘显示主窗口
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
    QMenu *fileMenu = menuBar()->addMenu("文件");
    QAction *newAction = new QAction("新建", fileMenu);
    QAction *quitAction = new QAction("退出", fileMenu);
    connect(newAction, &QAction::triggered, [&] { detailArea->reset(candyList->count() == 0); });
    connect(quitAction, &QAction::triggered, [&] {
        // 新建的时候可能会误触退出,再确认一次
        if (QMessageBox::question(this, "退出", "退出后将断开所有虚拟网络连接,确认退出?") == QMessageBox::Yes) {
            directQuit();
        }
    });

    fileMenu->addAction(newAction);
    fileMenu->addAction(quitAction);
}

void MainWindow::addEditMenu()
{
    QMenu *settingMenu = menuBar()->addMenu("编辑");
    QAction *autoStartAction = new QAction("启动选项", settingMenu);

    StartOption *startOption = new StartOption(this);
    connect(autoStartAction, &QAction::triggered, startOption, &StartOption::show);

    settingMenu->addAction(autoStartAction);
}

void MainWindow::addHelpMenu()
{
    QMenu *helpMenu = menuBar()->addMenu("帮助");

    QAction *feedbackAction = new QAction("问题反馈", helpMenu);
    Feedback *feedback = new Feedback(this);

    QAction *aboutAction = new QAction("关于", helpMenu);
    About *about = new About(this);

    connect(feedbackAction, &QAction::triggered, feedback, &Feedback::show);
    connect(aboutAction, &QAction::triggered, about, &About::show);

    helpMenu->addAction(feedbackAction);
    helpMenu->addAction(aboutAction);
}

void MainWindow::addCentralWidget()
{
    candyList->setParent(this);
    detailArea->setParent(this);

    // 右侧保存时追加左侧列表
    detailArea->setCandyList(candyList);

    // 将列表和详细信息区域添加到主窗口
    QWidget *placeholder = new QWidget(this);
    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(candyList, 1);
    layout->addWidget(detailArea, 2);
    placeholder->setLayout(layout);
    setCentralWidget(placeholder);

    centralWidget()->setLayout(layout);

    // 连接列表的 itemClicked() 信号到详细信息区域的更新函数
    connect(candyList, &QListWidget::itemClicked, detailArea, &DetailArea::selectItem);

    // 状态栏显示当前地址
    connect(detailArea, &DetailArea::updateTitle, this, &MainWindow::setWindowTitle);
}

void MainWindow::addSystemTray()
{
    // 添加系统托盘
    if (QSystemTrayIcon::isSystemTrayAvailable()) {
        QMenu *trayMenu = new QMenu(this);
        QIcon quitIcon = QApplication::style()->standardIcon(QStyle::SP_TabCloseButton);
        QAction *quitAction = new QAction(quitIcon, "退出", trayMenu);
        connect(quitAction, &QAction::triggered, this, &MainWindow::directQuit);

        trayMenu->addAction(quitAction);

        trayIcon = new QSystemTrayIcon(this);
        connect(trayIcon, &QSystemTrayIcon::activated, this, &MainWindow::onSystemTrayActivated);
        trayIcon->setIcon(QIcon(":/logo.ico"));
        trayIcon->setToolTip("组网工具");
        trayIcon->setContextMenu(trayMenu);
        trayIcon->show();
    }
}
