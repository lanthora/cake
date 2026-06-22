#ifndef DETAILAREA_H
#define DETAILAREA_H

#include <QComboBox>
#include <QFrame>
#include <QLabel>
#include <QLineEdit>
#include <QMap>
#include <QPushButton>
#include <QScrollArea>
#include <QSettings>
#include <QVBoxLayout>

class CandyItem;

class DetailArea : public QFrame
{
    Q_OBJECT

public:
    explicit DetailArea();
    ~DetailArea();

public slots:
    void selectItem(const QString &name);
    void reset(bool fillDefault = false);
    void save();
    void remove();

signals:
    void updateTitle(const QString &text);

private:
    QComboBox *selector;
    QScrollArea *scrollArea;
    QWidget *detailWidget;

    QLineEdit *websocket;
    QLineEdit *password;
    QLineEdit *tun;
    QLineEdit *stun;
    QLineEdit *port;
    QLineEdit *discovery;
    QLineEdit *route;
    QLineEdit *localhost;
    QLineEdit *mtu;
    QPushButton *removeButton;
    QPushButton *saveButton;

    QMap<QString, CandyItem *> items;

    QWidget *createInputRow(const QString &label, QWidget *input);
    QWidget *createButtonRow();

    QSettings settings;
};

#endif // DETAILAREA_H
