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
#include <QStackedWidget>
#include <QVBoxLayout>

class CandyItem;
class QTimer;

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
    QLineEdit *newName;
    QStackedWidget *networkStack;
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
    QLineEdit *ipAddress;
    QPushButton *removeButton;
    QPushButton *saveButton;
    QPushButton *cancelButton;

    QMap<QString, CandyItem *> items;
    QMap<QString, QString> cachedAddress;

    QWidget *createInputRow(const QString &label, QWidget *input);
    QWidget *createButtonRow();

    QString lastSelected;
    QTimer *statusTimer;

private slots:
    void pollStatus();

private:
    QSettings settings;
};

#endif // DETAILAREA_H
