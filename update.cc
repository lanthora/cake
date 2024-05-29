#include "update.h"
#include "define.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QSettings>
#include <QString>

Update::Update(QObject *parent)
    : QObject(parent)
{
    manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished, this, &Update::handleReply);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, [&] { check(); });
    timer->setInterval(10000);
    timer->start();
}

void Update::check()
{
    QSettings settings;
    if (settings.value("updatecheck", true).toBool()) {
        manager->get(QNetworkRequest(QUrl("https://api.github.com/repos/lanthora/cake/releases/latest")));
    } else {
        timer->stop();
    }
}

void Update::handleReply(QNetworkReply *reply)
{
    if (reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() == 200) {
        QByteArray response = reply->readAll();

        QString version = [&] {
            QJsonDocument doc = QJsonDocument::fromJson(response);
            if (doc.isObject()) {
                QJsonObject obj = doc.object();
                return obj["tag_name"].toString();
            }
            return QString();
        }();

        if (version.isEmpty()) {
            return;
        }
        if (timer->isActive()) {
            timer->stop();
            if (QString(version) != QString(CAKE_VERSION)) {
                notify(QString(CAKE_VERSION), QString(version));
            }
        }
    }

    reply->deleteLater();
}
