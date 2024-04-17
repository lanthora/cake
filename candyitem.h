#ifndef CANDYITEM_H
#define CANDYITEM_H

#include <map>
#include <mutex>
#include <thread>
#include <QListWidgetItem>
#include <queue>

class CandyItem : public QListWidgetItem
{
public:
    explicit CandyItem();
    ~CandyItem();

    void update();
    void shutdown();

private:
    std::shared_ptr<void> candy;
    int randomHex();
    QString randomHexString(int length);

public:
    static void startKeepAlive();
    static void stopKeepAlive();

private:
    friend void candyErrorCallback(void *candy);
    static std::thread keepAliveThread;
    static std::queue<std::weak_ptr<void>> candyQueue;
    static std::mutex candyMutex;
    static std::map<void *, std::weak_ptr<void>> candyRawWeakMap;
    static volatile bool running;
};

#endif // CANDYITEM_H
