#ifndef UPDATE_H
#define UPDATE_H

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
    QNetworkAccessManager *manager;
    QTimer *timer;
};

#endif // UPDATE_H
