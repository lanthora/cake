#ifndef ABOUT_H
#define ABOUT_H

#include <QDialog>
#include <QObject>

class About : public QDialog
{
    Q_OBJECT
public:
    About(QWidget *parent = nullptr);
};

#endif
