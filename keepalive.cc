#include "keepalive.h"
#include "candy.h"
#include <algorithm>
#include <QSysInfo>

KeepAlive &KeepAlive::instance()
{
    static KeepAlive i;
    return i;
}

void KeepAlive::add(std::shared_ptr<void> candy)
{
    std::lock_guard lock(this->mutex);
    this->map[candy.get()] = candy;
}

void KeepAlive::del(std::shared_ptr<void> candy)
{
    std::lock_guard lock(this->mutex);
    this->map.erase(candy.get());
}

void KeepAlive::restart(std::shared_ptr<void> candy)
{
    std::lock_guard lock(this->mutex);
    this->list.push_back(candy);
}

KeepAlive::KeepAlive()
{
    candy_init();

#ifdef Q_OS_WIN
    candy_set_log_path("C:/ProgramData/Cake/logs/candy.txt");
#endif

    candy_enable_debug();

    running = true;
    this->thread = std::thread([&] {
        while (running) {
            std::shared_ptr<void> candy;
            std::this_thread::sleep_for(std::chrono::seconds(1));

            if (running) {
                std::lock_guard lock(mutex);
                if (list.empty()) {
                    continue;
                }
                candy = list.front().lock();
                list.pop_front();
            }

            if (candy.use_count() > 1) {
                candy_client_shutdown(candy.get());
                candy_client_run(candy.get());
            }

            if (candy.use_count() == 1) {
                candy_client_shutdown(candy.get());
            }
        }
    });
    candy_client_set_error_cb(candy_error_cb);
}

KeepAlive::~KeepAlive()
{
    this->running = false;
    if (this->thread.joinable()) {
        this->thread.join();
    }
    candy_release();
}

void candy_error_cb(void *candy)
{
    KeepAlive &instance = KeepAlive::instance();
    std::lock_guard lock(instance.mutex);

    auto it = instance.map.find(candy);
    if (it == instance.map.end()) {
        return;
    }

    auto equal = [&](std::weak_ptr<void> ptr) { return !ptr.owner_before(it->second) && !it->second.owner_before(ptr); };
    if (std::find_if(instance.list.begin(), instance.list.end(), equal) != instance.list.end()) {
        return;
    }

    instance.list.push_back(it->second);
}
