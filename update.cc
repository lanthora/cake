#include "update.h"
#include <algorithm>
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
    timer->setInterval(INTERVAL_INIT);
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
    if (reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() != 200) {
        updateTimer();
        reply->deleteLater();
        return;
    }

    QByteArray response = reply->readAll();
    QString latestVersion = [&] {
        QJsonDocument doc = QJsonDocument::fromJson(response);
        if (doc.isObject()) {
            QJsonObject obj = doc.object();
            return obj["tag_name"].toString();
        }
        return QString();
    }();

    if (latestVersion.isEmpty()) {
        updateTimer();
        reply->deleteLater();
        return;
    }

    if (QString(latestVersion) != this->notifiedVersion) {
        this->notifiedVersion = QString(latestVersion);
        notify(QString(CAKE_VERSION), QString(latestVersion));
    }

    updateTimer(INTERVAL_ONEDAY);
    reply->deleteLater();
}

void Update::updateTimer(int interval)
{
    if (interval) {
        timer->setInterval(interval);
        return;
    }
    if (timer->interval() == INTERVAL_ONEDAY) {
        timer->setInterval(INTERVAL_INIT);
        return;
    }
    timer->setInterval(std::min(timer->interval() * 2, INTERVAL_LIMIT));
    return;
}
