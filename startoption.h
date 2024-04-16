#ifndef STARTOPTION_H
#define STARTOPTION_H

#include <QCheckBox>
#include <QDialog>
#include <QObject>
#include <QSettings>

class StartOption : public QDialog
{
    Q_OBJECT
public:
    StartOption(QWidget *parent = nullptr);

private:
    QCheckBox *autoStartup = new QCheckBox("开机自启动", this);
    QCheckBox *showMainWindow = new QCheckBox("显示主界面", this);
    QSettings settings;
    void save();
};

#endif // STARTOPTION_H
