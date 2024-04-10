#ifndef DETAILAREA_H
#define DETAILAREA_H

#include "candyitem.h"
#include "candylist.h"
#include <QFrame>
#include <QLabel>
#include <QLineEdit>
#include <QListWidgetItem>
#include <QPushButton>
#include <QSettings>
#include <QVBoxLayout>

class DetailArea : public QFrame
{
    Q_OBJECT

public:
    explicit DetailArea();
    void setCandyList(CandyList *candyList);
    void selectItem(QListWidgetItem *item);
    void reset();
    void save();
    void remove();

private:
    QWidget *detailWidget = nullptr;

private:
    QLineEdit *name = new QLineEdit;
    QLineEdit *websocket = new QLineEdit;
    QLineEdit *password = new QLineEdit;
    QLineEdit *tun = new QLineEdit;
    QLineEdit *stun = new QLineEdit;
    QLineEdit *port = new QLineEdit;
    QLineEdit *discovery = new QLineEdit;
    QLineEdit *route = new QLineEdit;
    QLineEdit *localhost = new QLineEdit;
    QPushButton *removeButton = new QPushButton("删除");
    QPushButton *saveButton = new QPushButton("保存");
    QWidget *createInputWidget(QString label, QLineEdit *input);
    QWidget *createSaveButton();

private:
    CandyList *candyList = nullptr;
    QSettings *settings = new QSettings("canets", "cake");
};

#endif // DETAILAREA_H
