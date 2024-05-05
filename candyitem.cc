#include "candyitem.h"
#include "candy.h"
#include "keepalive.h"
#include <QSettings>

void address_update_callback(const char *name, const char *address)
{
    QSettings settings;
    settings.beginGroup(name);
    settings.setValue("expected", address);
    settings.endGroup();
    settings.sync();
}

CandyItem::CandyItem()
{
    setTextAlignment(Qt::AlignCenter);
    setSizeHint(QSize(0, 40));

    candy = std::shared_ptr<void>(candy_client_create(), candy_client_release);
    KeepAlive::instance().add(candy);
}

CandyItem::~CandyItem()
{
    KeepAlive::instance().del(candy);
    candy_client_shutdown(candy.get());
}

void CandyItem::update()
{
    auto toStdString = [](QVariant value) { return value.toString().toStdString(); };
    QSettings settings;

    candy_client_set_name(candy.get(), toStdString(text()).c_str());
    candy_client_set_virtual_mac(candy.get(), toStdString(settings.value("vmac")).c_str());

    settings.beginGroup(text());
    candy_client_set_password(candy.get(), toStdString(settings.value("password")).c_str());
    candy_client_set_websocket_server(candy.get(), toStdString(settings.value("websocket")).c_str());
    candy_client_set_tun_address(candy.get(), toStdString(settings.value("tun")).c_str());
    candy_client_set_expected_address(candy.get(), toStdString(settings.value("expected")).c_str());
    candy_client_set_stun(candy.get(), toStdString(settings.value("stun")).c_str());
    candy_client_set_discovery_interval(candy.get(), settings.value("discovery").toInt());
    candy_client_set_route_cost(candy.get(), settings.value("route").toInt());
    candy_client_set_udp_bind_port(candy.get(), settings.value("port").toInt());
    candy_client_set_localhost(candy.get(), toStdString(settings.value("localhost")).c_str());
    settings.endGroup();

    candy_client_set_address_update_callback(candy.get(), address_update_callback);
    KeepAlive::instance().restart(candy);
}
