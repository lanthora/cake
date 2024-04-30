#include "update.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QSettings>
#include <QString>

#ifndef CAKE_VERSION
#define CAKE_VERSION "Unknown"
#endif

Update::Update(QObject *parent)
    : QObject(parent)
{
    manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished, this, &Update::handleReply);

    // 启动时以静默模式检查更新,除非有可用升级,否则不显示任何信息
    QSettings settings;
    if (settings.value("updatecheck", true).toBool()) {
        check();
    }
}

void Update::check()
{
    manager->get(QNetworkRequest(QUrl("https://api.github.com/repos/lanthora/cake/releases/latest")));
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

        if (QString(version) != QString(CAKE_VERSION)) {
            notify(QString(CAKE_VERSION), QString(version));
        }
    }

    reply->deleteLater();
}
