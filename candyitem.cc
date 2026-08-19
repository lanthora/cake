#include "candyitem.h"
#include "candy/candy.h"
#include <Poco/JSON/Object.h>
#include <Poco/JSON/Stringifier.h>
#include <QSettings>
#include <chrono>

CandyItem::CandyItem(const QString &name)
    : m_name(name)
    , id(name.toStdString()) {}

CandyItem::~CandyItem()
{
    if (started) {
        candy::client::shutdown(id);
        if (clientThread.joinable()) {
            clientThread.join();
        }
    }
}

void CandyItem::update()
{
    QSettings settings;
    QString vmac = settings.value("vmac").toString();

    settings.beginGroup(m_name);
    Poco::JSON::Object config;
    config.set("name", id);
    config.set("vmac", vmac.toStdString());
    config.set("password", settings.value("password").toString().toStdString());
    config.set("websocket", settings.value("websocket").toString().toStdString());
    config.set("tun", settings.value("tun").toString().toStdString());
    config.set("stun", settings.value("stun").toString().toStdString());
    config.set("discovery", settings.value("discovery", "300").toInt());
    config.set("route", settings.value("route", "5").toInt());
    config.set("port", settings.value("port", "0").toInt());
    config.set("localhost", settings.value("localhost").toString().toStdString());
    config.set("mtu", settings.value("mtu", "1400").toInt());

    QString expt = settings.value("expt", settings.value("expected")).toString();
    config.set("expt", expt.toStdString());
    if (settings.contains("expected") && !settings.contains("expt")) {
        settings.setValue("expt", settings.value("expected"));
        settings.remove("expected");
    }

    settings.endGroup();
    settings.sync();

    std::thread previous;
    std::shared_ptr<std::atomic<bool>> previousDone;
    if (started) {
        previous = std::move(clientThread);
        previousDone = std::move(clientDone);
    }
    started = true;

    auto done = std::make_shared<std::atomic<bool>>(false);
    clientDone = done;

    const std::string idCopy = id;
    clientThread = std::thread(
        [id = idCopy, config, done,
         previous = std::move(previous),
         previousDone = std::move(previousDone)]() mutable {
            if (previous.joinable()) {
                while (!previousDone->load()) {
                    candy::client::shutdown(id);
                    std::this_thread::sleep_for(std::chrono::milliseconds(200));
                }
                previous.join();
            }
            struct DoneGuard {
                std::shared_ptr<std::atomic<bool>> d;
                explicit DoneGuard(std::shared_ptr<std::atomic<bool>> d)
                    : d(std::move(d))
                {
                }
                ~DoneGuard() { d->store(true); }
            } guard(done);
            candy::client::run(id, config);
        });
}
