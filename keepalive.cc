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
    candy_use_system_time();

#ifdef Q_OS_WIN
    candy_set_log_path("C:/ProgramData/Cake/logs/daily");
#endif

    candy_enable_debug();

    running = true;
    this->thread = std::thread([&] {
        while (running) {
            std::shared_ptr<void> candy;
            std::this_thread::sleep_for(std::chrono::seconds(1));

            // 列表里取出一个待重启的客户端
            if (running) {
                std::lock_guard lock(mutex);
                if (!list.empty()) {
                    candy = list.front().lock();
                    list.pop_front();
                }
            }

            // 重启客户端过程中不能加锁,否则调用 candy_error_cb 时会产生死锁
            if (candy) {
                candy_client_shutdown(candy.get());
                candy_client_run(candy.get());
            }

            // 重启过程中,可能存在上层释放了 candy 的可能,如果上层还持有共享指针,就释放这个共享指针
            if (candy) {
                std::lock_guard lock(mutex);
                if (this->map.find(candy.get()) != this->map.end()) {
                    candy.reset();
                }
            }

            // 上一步没有释放掉共享指针,意味着这是最后一个持有指针的对象,应该关闭客户端,并释放内存
            if (candy) {
                candy_client_shutdown(candy.get());
                candy.reset();
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
