#include "candyitem.h"
#include "candy.h"
#include <iomanip>
#include <iostream>
#include <random>
#include <sstream>
#include <QSettings>

std::thread CandyItem::keepAliveThread;
std::queue<void *> CandyItem::candyQueue;
bool CandyItem::running;

void address_update_callback(const char *name, const char *address)
{
    QSettings settings("canets", "cake");
    settings.beginGroup(name);
    settings.setValue("expected", address);
    settings.endGroup();
    settings.sync();
}

CandyItem::CandyItem()
{
    setTextAlignment(Qt::AlignCenter);
    setSizeHint(QSize(0, 40));

    candy = candy_client_create();
}

CandyItem::~CandyItem()
{
    candy_client_release(candy);
}

void CandyItem::update()
{
    auto toStdString = [](QVariant value) { return value.toString().toStdString(); };
    QSettings settings("canets", "cake");

    candy_client_set_name(candy, toStdString(text()).c_str());

    settings.beginGroup(text());

    if (settings.value("vmac").isNull()) {
        settings.setValue("vmac", randomHexString(16));
    }

    candy_client_set_virtual_mac(candy, toStdString(settings.value("vmac")).c_str());
    candy_client_set_password(candy, toStdString(settings.value("password")).c_str());
    candy_client_set_websocket_server(candy, toStdString(settings.value("websocket")).c_str());
    candy_client_set_tun_address(candy, toStdString(settings.value("tun")).c_str());
    candy_client_set_expected_address(candy, toStdString(settings.value("expected")).c_str());
    candy_client_set_stun(candy, toStdString(settings.value("stun")).c_str());
    candy_client_set_discovery_interval(candy, settings.value("discovery").toInt());
    candy_client_set_route_cost(candy, settings.value("route").toInt());
    candy_client_set_udp_bind_port(candy, settings.value("port").toInt());
    candy_client_set_localhost(candy, toStdString(settings.value("localhost")).c_str());

    settings.endGroup();

    candy_client_set_address_update_callback(candy, address_update_callback);

    CandyItem::candyQueue.push(candy);
}

void CandyItem::shutdown()
{
    candy_client_shutdown(candy);
}

int CandyItem::randomHex()
{
    std::random_device device;
    std::mt19937 engine(device());
    std::uniform_int_distribution<int> distrib(0, 15);
    return distrib(engine);
}

QString CandyItem::randomHexString(int length)
{
    QString retval;
    QTextStream out(&retval);
    std::stringstream ss;
    for (int i = 0; i < length; i++) {
        out << QString::number(randomHex(), 16);
    }
    return retval;
}

void candyErrorCallback(void *candy)
{
    CandyItem::candyQueue.push(candy);
}

void CandyItem::startKeepAlive()
{
    running = true;
    candy_client_use_system_time();
    keepAliveThread = std::thread([&] {
        while (running) {
            if (!candyQueue.empty()) {
                void *candy = candyQueue.front();
                candy_client_shutdown(candy);
                candy_client_run(candy);
                candyQueue.pop();
            }
            std::this_thread::sleep_for(std::chrono::seconds(3));
        }
    });
    candy_client_set_error_cb(candyErrorCallback);
}

void CandyItem::stopKeepAlive()
{
    running = false;
    if (keepAliveThread.joinable()) {
        keepAliveThread.join();
    }
}
