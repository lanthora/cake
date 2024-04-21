#ifndef KEEPALIVE_H
#define KEEPALIVE_H

#include "candy.h"
#include <list>
#include <map>
#include <memory>
#include <mutex>
#include <thread>

void candy_error_cb(void *candy);

class KeepAlive
{
public:
    static KeepAlive &instance();
    void add(std::shared_ptr<void> candy);
    void del(std::shared_ptr<void> candy);
    void restart(std::shared_ptr<void> candy);

private:
    KeepAlive();
    ~KeepAlive();

    std::mutex mutex;
    std::thread thread;
    std::list<std::weak_ptr<void>> list;
    std::map<void *, std::weak_ptr<void>> map;
    bool running;

    friend void candy_error_cb(void *candy);
};

#endif
