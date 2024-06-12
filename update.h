#ifndef UPDATE_H
#define UPDATE_H

#include "define.h"
#include <QNetworkAccessManager>
#include <QObject>
#include <QTimer>

class Update : public QObject
{
    Q_OBJECT

public:
    Update(QObject *parent = nullptr);

signals:
    void notify(QString current, QString latest);

public slots:
    void handleReply(QNetworkReply *reply);
    void check();

private:
    void updateTimer(int interval = 0);
    QNetworkAccessManager *manager;
    QTimer *timer;
    QString notifiedVersion = CAKE_VERSION;
    const int INTERVAL_INIT = 3000;
    const int INTERVAL_LIMIT = 3600000;
    const int INTERVAL_ONEDAY = 86400000;
};

#endif // UPDATE_H
