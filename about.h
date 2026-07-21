#ifndef ABOUT_H
#define ABOUT_H

#include "framelessdialog.h"
#include <QObject>

class About : public FramelessDialog
{
    Q_OBJECT
public:
    About(QWidget *parent = nullptr);
};

#endif
