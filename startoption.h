#ifndef STARTOPTION_H
#define STARTOPTION_H

#include "framelessdialog.h"
#include <QCheckBox>
#include <QObject>
#include <QSettings>

class StartOption : public FramelessDialog
{
    Q_OBJECT
public:
    StartOption(QWidget *parent = nullptr);

protected:
    void showEvent(QShowEvent *event) override;

private:
    QCheckBox *autoStartup = new QCheckBox("Start on boot", this);
    QCheckBox *showMainWindow = new QCheckBox("Show main window", this);
    QCheckBox *updateCheck = new QCheckBox("Check for updates", this);
    QSettings settings;
    void save();
};

#endif // STARTOPTION_H
