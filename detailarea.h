#ifndef DETAILAREA_H
#define DETAILAREA_H

#include <QFrame>
#include <QLabel>
#include <QLineEdit>
#include <QListWidgetItem>
#include <QVBoxLayout>

class DetailArea : public QFrame
{
    Q_OBJECT

public:
    explicit DetailArea();
    void update(QListWidgetItem *item);
    void save();

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
    QWidget *createInputWidget(QString label, QLineEdit *input);
    QWidget *createSaveButton();
};

#endif // DETAILAREA_H
