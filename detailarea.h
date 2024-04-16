#ifndef DETAILAREA_H
#define DETAILAREA_H

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
    QLineEdit *name = new QLineEdit(this);
    QLineEdit *websocket = new QLineEdit(this);
    QLineEdit *password = new QLineEdit(this);
    QLineEdit *tun = new QLineEdit(this);
    QLineEdit *stun = new QLineEdit(this);
    QLineEdit *port = new QLineEdit(this);
    QLineEdit *discovery = new QLineEdit(this);
    QLineEdit *route = new QLineEdit(this);
    QLineEdit *localhost = new QLineEdit(this);
    QPushButton *removeButton = new QPushButton("删除", this);
    QPushButton *saveButton = new QPushButton("保存", this);
    QWidget *createInputWidget(QString label, QLineEdit *input);
    QWidget *createSaveButton();

private:
    CandyList *candyList = nullptr;
    QSettings settings = QSettings("canets", "cake");
};

#endif // DETAILAREA_H
