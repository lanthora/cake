#ifndef UPDATE_H
#define UPDATE_H

#include <QNetworkAccessManager>
#include <QObject>

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
};

#endif // UPDATE_H
